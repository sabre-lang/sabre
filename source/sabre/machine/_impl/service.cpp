/// Sabre Includes
#include "sabre/machine/service.hpp"
#include "sabre/function/common.hpp"
#include "sabre/machine/visitor.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/runtime/isolate.hpp"

//  CONSTRUCTORS  //

Sabre::Machine::Service::Service() : Service($::Global::get<Runtime::Container>()) {}
Sabre::Machine::Service::Service(XI::Container *services) : m_services(services) {}

//  PUBLIC METHODS  //

bool Sabre::Machine::Service::verify(const Function::Info *info) const noexcept {
  auto jitless = m_services->get<Runtime::Options>()->flags.jitless;
  return !jitless && info->locals() <= XASM::Constants::VREGS_MAX;
}

Sabre::Function::Any Sabre::Machine::Service::upgrade(Runtime::Isolate *isolate, const Function::Closure &closure) {
  // attempt verifying the instance now
  auto *info = closure.info();
  if (!verify(info)) return closure;

  // get all the necessary services required
  bool logging = m_services->get<Runtime::Options>()->dump.assembly;

  // prepare a compilation scope to be used
  auto compiler = m_context.scope<Callback>(logging);

  // construct a new callee information to be used now
  auto *callee = m_callees.emplace($::Unique::New<Info>(info)).first->get();

  // attempt compilation now as necessary
  m_compile(callee, compiler.get());
  callee->callback() = compiler->finish();
  callee->bytes() = m_context.arena()->code_size();

  // show logging results if necessary
  if ($_UNLIKELY(logging)) m_dump(info);

  // prepare the receiver and context to be used
  auto receiver = closure.receiver();
  auto &context = closure.context();

  // and construct the resulting native function to be used
  return isolate->create<Function::Jitted>(callee, receiver, context);
}

//  PRIVATE METHODS  //

void Sabre::Machine::Service::m_dump(const Function::Info *info) const noexcept {
  // get the current details of the function
  auto bytes = m_context.arena()->code_size();
  auto relative = info->resource().relative();

  // show the baseline dump details now
  $::Debug::println("\n===== Assembly Dump / {0}B '{1}' =====\n", bytes, relative.string());

  // and attempt showing the necessary output now
  if (auto content = m_context.content(); content.size()) $::Debug::println(content);
}

void Sabre::Machine::Service::m_compile(Info *callee, Compiler *compiler) const noexcept {
  // get the underlying bytecode to be converted
  auto bytecode = callee->bytecode();

  // prepare some details about the bytecode
  auto offset = bytecode.address();
  auto labels = bytecode.size() / Bytecode::Width;

  // prepare the scoped builder process now
  auto builder = m_services->get<Builder>(callee, compiler);

  // reserve the total labels now to be used
  for (size_t ii = 0; ii < labels; ++ii) builder->labels.emplace_back(compiler->new_label());

  // prepare the base instruction instance to be iterated over
  const auto *instruction = std::bit_cast<const Bytecode::Instruction *>(offset);

  // iterate over the bytecode to be transformed
  for (size_t ii = 0; ii < labels; ++ii, ++instruction) {
    builder->emitter->header(instruction, ii);
    Visitor::accept(builder.get(), instruction);
  }
}
