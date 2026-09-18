/// Sabre Includes
#include "sabre/machine/builder.hpp"
#include "sabre/machine/info.hpp"
#include "sabre/number/tagged.hpp"

/// Machine Includes
#include "sabre/machine/_inline/glue.ipp"

//  CONSTRUCTORS  //

Sabre::Machine::Emitter::Emitter(Builder *builder) : m_builder(builder), m_compiler(builder->compiler) {}

//  PUBLIC METHODS  //

Sabre::Machine::Label Sabre::Machine::Emitter::label(const Bytecode::Index &index) {
  auto *arena = m_builder->info->arena();
  auto constant = arena->constants[index];
  size_t jump = constant.as<Number::Tagged>();
  auto offset = m_offset + (jump / Bytecode::Width);
  return m_builder->labels[offset + 1]; // bump offset
}

void Sabre::Machine::Emitter::header(const Bytecode::Instruction *instruction, size_t offset) {
  // prepare the baseline header instance
  m_builder->logger->header(instruction->label());
  m_compiler->bind(m_builder->labels[m_offset = offset]);

  // stop if no need to panic at all
  if (!offset || !m_builder->facts.panics()) return;

  // prepare a temporary register for bumping
  auto tx = m_compiler->new_gp64();

  // we need to store the current offset value now
  m_compiler->mov(tx, offset * Bytecode::Width);
  m_compiler->store_u64(mem(m_builder->feedback), tx);
}

Sabre::Register::Host Sabre::Machine::Emitter::slot(const Register::Slot &vreg) {
  // ensure the register is suitably bounded
  $_ASSERT(!vreg.nowhere(), "Unbounded register value");

  // stop early if we have the accumulator register to be returned
  if (vreg.accumulator()) return m_builder->result;

  // check if we have a local value instead of an argument
  auto local = (vreg.encode() - 1) > m_builder->info->argmax();
  if (local) return m_builder->registers->allocate(vreg);

  // otherwise stop early if the register has already been bound
  if (m_builder->registers->bound(vreg)) return m_builder->registers->resolve(vreg);

  // since we have an unbounded argument, we must bind
  auto dst = m_builder->registers->allocate(vreg);
  return m_argv(dst, vreg.encode() - 1), dst;
}

Sabre::Machine::Immediate Sabre::Machine::Emitter::ref(const Value::Any &value) const noexcept { return &value; }
Sabre::Machine::Immediate Sabre::Machine::Emitter::imm(const Value::Any &value) const noexcept {
  return value.pointer().value();
}

Sabre::Machine::Memory Sabre::Machine::Emitter::mem(const Register::Host &vreg, size_t offset) const noexcept {
  return Memory(vreg, offset);
}

void Sabre::Machine::Emitter::self(const Register::Slot &vreg) { self(slot(vreg)); }
void Sabre::Machine::Emitter::self(const Register::Host &vreg) {
  static constexpr auto s_offset = sizeof(Value::Any) * Function::Offset::ARGS_SELF;
  m_compiler->load_u64(vreg, mem(m_builder->argv, s_offset)); // use the self offset
}

void Sabre::Machine::Emitter::load(const Value::Any &value) { load(m_builder->result, value); }
void Sabre::Machine::Emitter::load(const Register::Host &vreg, const Value::Any &value) { load(vreg, imm(value)); }
void Sabre::Machine::Emitter::load(const Register::Host &vreg, const Immediate &value) { m_compiler->mov(vreg, value); }
void Sabre::Machine::Emitter::load(const Register::Slot &vreg, const Value::Any &value) { load(slot(vreg), value); }
void Sabre::Machine::Emitter::load(const Register::Slot &vreg, const Immediate &value) { load(slot(vreg), value); }
void Sabre::Machine::Emitter::load(const Bytecode::Instruction *unqualified, uint16_t depth) {
  // prepare the instruction to be used for loading
  auto *instruction = unqualified->cast<Bytecode::Glyph::LOAD_CONTEXT>();

  // get the encessary items to be used
  auto vreg = instruction->get<0>();
  auto ireg = instruction->get<1>();

  // and assign out necessary details now
  if (!depth) call(Glue::ldar, slot(vreg), m_builder->envp, ireg.encode());
  else call(Glue::ldup, slot(vreg), m_builder->envp, ireg.encode(), depth);
}

void Sabre::Machine::Emitter::move(const Register::Slot &dst, const Register::Slot &src) { move(slot(dst), slot(src)); }
void Sabre::Machine::Emitter::move(const Register::Slot &dst, const Register::Host &src) { move(slot(dst), src); }
void Sabre::Machine::Emitter::move(const Register::Host &dst, const Register::Slot &src) { move(dst, slot(src)); }
void Sabre::Machine::Emitter::move(const Register::Host &dst, const Register::Host &src) { m_compiler->mov(dst, src); }
void Sabre::Machine::Emitter::move(const Bytecode::Instruction *unqualified, uint16_t depth) {
  // prepare the instruction to be used for loading
  auto *instruction = unqualified->cast<Bytecode::Glyph::STORE_CONTEXT>();

  // get the encessary items to be used
  auto vreg = instruction->get<0>();
  auto ireg = instruction->get<1>();

  // and assign out necessary details now
  if (!depth) call(Glue::star, m_builder->envp, slot(vreg), ireg.encode());
  else call(Glue::stup, m_builder->envp, slot(vreg), ireg.encode(), depth);
}

void Sabre::Machine::Emitter::swap(const Register::Slot &dst, const Register::Slot &src) { swap(slot(dst), slot(src)); }
void Sabre::Machine::Emitter::swap(const Register::Slot &dst, const Register::Host &src) { swap(slot(dst), src); }
void Sabre::Machine::Emitter::swap(const Register::Host &dst, const Register::Slot &src) { swap(dst, slot(src)); }
void Sabre::Machine::Emitter::swap(const Register::Host &dst, const Register::Host &src) {
#if defined(ASMJIT_UJIT_X86)
  m_compiler->cc->xchg(dst, src);
#else
  // prepare a temporary register
  auto tmp = m_compiler->new_gp64();

  // and swap as necessary
  m_compiler->mov(tmp, dst);
  m_compiler->mov(dst, src);
  m_compiler->mov(src, tmp);
#endif
}

void Sabre::Machine::Emitter::params(const Register::Span &span) { params({}, span); }
void Sabre::Machine::Emitter::params(const Register::Slot &self, const Register::Span &span) {
  // prepare a temporary register to be used
  auto tx = m_compiler->new_gp64();

  // prepare the baseline memory location now
  auto memory = mem(m_builder->params, Function::Offset::ARGS_SIZE);

  // prepare the size of the incoming arguments to be used
  load(tx, span.count()), m_compiler->store_u64(memory, tx);

  // upload the incoming self value as well
  self.nowhere() ? load(tx, Constants::Void) : move(tx, self);

  // set the incoming self value now
  memory.add_offset(sizeof(Value::Any)), m_compiler->store_u64(memory, tx);

  // finally push all the incoming arguments as well
  for (size_t ii = 0; ii < span.count(); ++ii) {
    memory.add_offset(sizeof(Value::Any)); // bump offset
    m_compiler->store_u64(memory, slot(span.first() + ii));
  }
}

void Sabre::Machine::Emitter::getter(const Register::Host &dx, const Register::Host &tx, const Bytecode::Index &index) {
  // prepare all the necessary getter arguments
  auto sx = ref(m_builder->info->arena()->constants[index]);

  // attempt getting the field required now
  call(Glue::getter, dx, m_builder->isolate, tx, sx);

  // post-validate the result was not a failure
  test(dx, Validate::SLOW);
}

void Sabre::Machine::Emitter::setter(const Register::Host &tx, const Register::Host &vx, const Bytecode::Index &index) {
  // we need a feedback register for results
  auto dx = m_compiler->new_gp64();

  // prepare all the necessary getter arguments
  auto sx = ref(m_builder->info->arena()->constants[index]);

  // attempt getting the field required now
  call(Glue::setter, dx, m_builder->isolate, tx, vx, sx);

  // post-validate the result was not a failure
  test(dx, Validate::SLOW);
}

void Sabre::Machine::Emitter::vtof(const Label &label, const Double &xmm, const Register::Host &vreg) {
  m_compiler->j(label, asmjit::ujit::bt_nz(vreg, 0));
  m_compiler->s_mov_u64(xmm, vreg); // value-to-float
}

void Sabre::Machine::Emitter::ftov(const Double &xmm, const Register::Host &vreg) {
  m_compiler->s_mov_u64(vreg, xmm); // float-to-value
  m_compiler->and_(vreg, vreg, ~Pointer::Mask::NUM);
}

void Sabre::Machine::Emitter::ret() { ret(m_builder->result); }
void Sabre::Machine::Emitter::ret(const Value::Any &value) { ret(m_builder->result, value); }
void Sabre::Machine::Emitter::ret(const Register::Host &vreg) { m_compiler->ret(vreg); }
void Sabre::Machine::Emitter::ret(const Register::Host &vreg, const Value::Any &value) { load(vreg, value), ret(vreg); }

void Sabre::Machine::Emitter::irq() { m_compiler->j(m_builder->interrupt); }
void Sabre::Machine::Emitter::irq(Engine::Interrupt mode) {
  // prepare a suitable test condition
  auto tx = m_compiler->new_gp64();
  auto flag = 1 << static_cast<uint8_t>(mode);
  auto failure = asmjit::ujit::test_nz(tx, flag);

  // prepare the mode that we want to compare against
  m_compiler->load_u64(tx, mem(m_builder->frame, $::Memory::Size::PTR));
  m_compiler->and_(tx, tx, Immediate(Pointer::Mask::IMM));

  // jump if we succeed the failure condition
  m_compiler->j(m_builder->interrupt, failure);
}

void Sabre::Machine::Emitter::test(const Register::Slot &vreg, Validate mode) { test(slot(vreg), mode); }
void Sabre::Machine::Emitter::test(const Register::Host &vreg, Validate mode) {
  // if we are given a fast-mode then we just check equality to failure values
  if (mode == Validate::FAST) return m_compiler->j(m_builder->panic, asmjit::ujit::cmp_eq(vreg, imm(Constants::Fail)));

  // prepare the required components for the test
  auto tx = m_compiler->new_gp64();
  auto failure = asmjit::ujit::cmp_eq(tx, Pointer::Mask::ERRC);

  // otherwise we need to make sure our feedback is valid
  m_compiler->and_(tx, vreg, Pointer::Mask::ERRC);
  m_compiler->j(m_builder->panic, failure);
}

void Sabre::Machine::Emitter::jmpt(const Label &label, const Register::Slot &vreg) { jmpt(label, slot(vreg)); }
void Sabre::Machine::Emitter::jmpt(const Label &label, const Register::Host &vreg) {
  // prepare the details for testing
  auto tx = m_compiler->new_gp64();
  auto failure = asmjit::ujit::test_nz(tx);

  // and attempt testing our jump
  call(Glue::truthiness, tx, vreg);
  m_compiler->j(label, failure);
}

void Sabre::Machine::Emitter::jmpf(const Label &label, const Register::Slot &vreg) { jmpf(label, slot(vreg)); }
void Sabre::Machine::Emitter::jmpf(const Label &label, const Register::Host &vreg) {
  // prepare the details for testing
  auto tx = m_compiler->new_gp64();
  auto failure = asmjit::ujit::test_z(tx);

  // and attempt testing our jump
  call(Glue::truthiness, tx, vreg);
  m_compiler->j(label, failure);
}

void Sabre::Machine::Emitter::jmpc(const Label &label, const Register::Slot &vreg, const Bytecode::Index &index) {
  return jmpc(label, vreg, m_builder->info->arena()->constants[index]);
}

void Sabre::Machine::Emitter::jmpc(const Label &label, const Register::Host &vreg, const Bytecode::Index &index) {
  return jmpc(label, vreg, m_builder->info->arena()->constants[index]);
}

void Sabre::Machine::Emitter::jmpc(const Label &label, const Register::Slot &vreg, const Value::Any &value) {
  jmpc(label, slot(vreg), value);
}

void Sabre::Machine::Emitter::jmpc(const Label &label, const Register::Host &vreg, const Value::Any &value) {
  m_compiler->j(label, asmjit::ujit::cmp_eq(vreg, imm(value)));
}

//  PRIVATE METHODS  //

void Sabre::Machine::Emitter::m_argv(const Register::Host &vreg, size_t offset) {
  offset = sizeof(Value::Any) * (offset + Function::Offset::ARGS_DATA);
  m_compiler->load_u64(vreg, mem(m_builder->argv, offset));
}
