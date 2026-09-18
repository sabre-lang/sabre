/// Sabre Modules
#include "sabre/bytecode/invoker.hpp"
#include "sabre/bytecode/compiler.hpp"
#include "sabre/register/slot.hpp"

//  PUBLIC METHODS  //

void Sabre::Bytecode::Invoker::compile(Compiler *compiler, Register::Slot destination, const Args &args) const {
  // prepare the dispatch handler now
  auto list = m_prepare(compiler, destination, args);

  // and dispatch the inocation based on the argument and protocol
  if (args.empty()) m_async ? m_spawn(compiler, destination) : m_invoke(compiler, destination);
  else m_async ? m_spawn(compiler, destination, list) : m_invoke(compiler, destination, list);
}

//  PRIVATE METHODS  //

Sabre::Register::List
Sabre::Bytecode::Invoker::m_prepare(Compiler *compiler, Register::Slot &destination, const Args &args) const {
  // prepare the args list to be used
  auto list = compiler->registers()->list();

  // fix the destination to be used now
  if (destination.nowhere()) destination = Register::Accumulator;

  // construct a suitable callee register to be used
  auto creg = args.empty() || inlined(m_async) ? Register::Accumulator : list.grow();

  // handle the callee value that should be lowered
  switch (m_convention) {
  case Convention::VOID: compiler->lower(m_callee, creg); break;
  case Convention::FIELD: compiler->lower(m_callee->as<Syntax::Accessor>()->parent(), creg); break;
  case Convention::INLINE: {
    // we only allow inline if not asynchronous
    if (m_async) compiler->lower(m_callee, creg);
  } break;
  }

  // otherwise attempt building with multiple values
  for (const auto &argument : args) compiler->lower(argument, list.grow());

  // return the resulting list now
  return list;
}

void Sabre::Bytecode::Invoker::m_invoke(Compiler *compiler, const Register::Slot &destination) const noexcept {
  switch (m_convention) {
  case Convention::VOID: compiler->emit<Glyph::CALL_0_VOID>(destination); break;
  case Convention::INLINE: compiler->emit<Glyph::CALL_0_INLINE>(destination); break;
  case Convention::FIELD: {
    $_UNUSED $_AUTO = compiler->trace(m_callee);
    auto field = m_callee->as<Syntax::Accessor>()->field(); // cast to suitable field
    compiler->emit<Glyph::CALL_0_FIELD>(destination, compiler->symbol(field->name()));
  } break;
  }
}

void Sabre::Bytecode::Invoker::m_invoke(
    Compiler *compiler, const Register::Slot &destination, const Register::Span &span
) const noexcept {
  switch (m_convention) {
  case Convention::VOID: compiler->emit<Glyph::CALL_N_VOID>(destination, span); break;
  case Convention::INLINE: compiler->emit<Glyph::CALL_N_INLINE>(destination, span); break;
  case Convention::FIELD: {
    $_UNUSED $_AUTO = compiler->trace(m_callee);
    auto field = m_callee->as<Syntax::Accessor>()->field(); // cast to a suitable field now
    compiler->emit<Glyph::CALL_N_FIELD>(destination, compiler->symbol(field->name()), span);
  } break;
  }
}

void Sabre::Bytecode::Invoker::m_spawn(Compiler *compiler, const Register::Slot &destination) const noexcept {
  switch (m_convention) {
  case Convention::VOID: $_FALLTHROUGH; // we do not have inline items when spawning
  case Convention::INLINE: compiler->emit<Glyph::SPAWN_0_VOID>(destination); break;
  case Convention::FIELD: {
    $_UNUSED $_AUTO = compiler->trace(m_callee);
    auto field = m_callee->as<Syntax::Accessor>()->field(); // cast to suitable field
    compiler->emit<Glyph::SPAWN_0_FIELD>(destination, compiler->symbol(field->name()));
  } break;
  }
}

void Sabre::Bytecode::Invoker::m_spawn(
    Compiler *compiler, const Register::Slot &destination, const Register::Span &span
) const noexcept {
  switch (m_convention) {
  case Convention::VOID: $_FALLTHROUGH; // we do not have inline items when spawning
  case Convention::INLINE: compiler->emit<Glyph::SPAWN_N_VOID>(destination, span); break;
  case Convention::FIELD: {
    $_UNUSED $_AUTO = compiler->trace(m_callee);
    auto field = m_callee->as<Syntax::Accessor>()->field(); // cast to a suitable field now
    compiler->emit<Glyph::SPAWN_N_FIELD>(destination, compiler->symbol(field->name()), span);
  } break;
  }
}
