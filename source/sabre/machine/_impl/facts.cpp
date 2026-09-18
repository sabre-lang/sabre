/// Sabre Includes
#include "sabre/machine/facts.hpp"
#include "sabre/bytecode/iterator.hpp"
#include "sabre/function/args.hpp"
#include "sabre/machine/info.hpp"

//  PRIVATE METHODS  //

void Sabre::Machine::Facts::m_deduce(Info *callee) {
  // for some items we need to iterate over the bytecode to deduce
  for (const auto &instruction : Bytecode::Iterator(callee->bytecode())) m_reserve(callee, instruction);

  // prepare some baseline statistics here
  m_argv = callee->adicity() > 0;
  m_params = callee->locals() > 0;
}

void Sabre::Machine::Facts::m_reserve(Info *callee, const Register::Span &span) {
  callee->locals() = std::max<size_t>(callee->locals(), span.count() + Function::Offset::ARGS_DATA);
}

void Sabre::Machine::Facts::m_reserve(Info *callee, const Bytecode::Instruction &instruction) {
  // prepare a type alias for destructing as needed
  using Glyph = Bytecode::Glyph;

  // check some base facts that we want to check for
  if (instruction.glyph().leaked()) m_envp = true;
  if (instruction.glyph().panics()) m_panics = true;

  // attempt resolving the required encoded instruction
  switch (instruction.glyph().encoded()) {
  // these values should ensure that the size is emitted
  case Glyph::CALL_0_VOID: m_reserve(callee, Register::Span()); break;
  case Glyph::CALL_0_FIELD: m_reserve(callee, Register::Span()); break;
  case Glyph::CALL_0_INLINE: m_reserve(callee, Register::Span()); break;

  // all of these items rely on an incoming sized span value
  case Glyph::CALL_N_VOID: m_reserve(callee, instruction.cast<Glyph::CALL_N_VOID>()->get<1>()); break;
  case Glyph::CALL_N_FIELD: m_reserve(callee, instruction.cast<Glyph::CALL_N_FIELD>()->get<2>().slice(1)); break;
  case Glyph::CALL_N_INLINE: m_reserve(callee, instruction.cast<Glyph::CALL_N_INLINE>()->get<1>()); break;

  // ignore handling for the default cases
  default: break;
  }
}
