/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"
#include "sabre/value/fold.hpp"

/// Forward Declarations
$_FWD(Sabre::Bytecode::Dispatch, bool constant(Compiler *, const Syntax::Variable *, const Value::Any &))

//  PUBLIC METHODS  //

bool Sabre::Bytecode::Dispatch::constant(Compiler *compiler, const Syntax::Variable *node, const Value::Any &value) {
  // many conditions to allow here
  if (!node->immutable()) return false;
  if (!value.pointer().okay()) return false;
  if (node->attributes().size()) return false;
  if (node->decorators().size()) return false;

  // since we reached here, we can declare constant (or attempt)
  bool valid = compiler->variables()->declare(node->name(), value);
  if (!valid) compiler->panic(4000403, node->name());
  else compiler->expose(node, Register::Accumulator, value);

  // since we reached here, declare as a success for constant propagation
  return true;
}

SABRE_MM_LOWER_NODE(Variable, node, compiler, ) {
  // prepare the trace for the node now
  $_UNUSED $_AUTO = compiler->trace(node);

  // pull out the incoming initializer value
  auto *initializer = node->initializer();
  auto value = Value::Fold(initializer);

  // if we have a suitable and clean constant, then we handle differently
  if (Bytecode::Dispatch::constant(compiler, node, value)) return;

  // get the location of the variable now
  auto [dest, leaked] = compiler->declare(node);

  // bind the initialize as necessary now
  auto ireg = leaked ? Register::Accumulator : dest;
  if (initializer) compiler->lower(initializer, ireg);

  // we want to update the current value details
  compiler->preamble(node, ireg);

  // if the instance is disposable, then attach to frame
  if (node->disposable()) compiler->emit<Glyph::DISPOSE_DEFER>(ireg);

  // and handle assignment based on the leakage state
  if (leaked) compiler->emit<Glyph::STORE_CONTEXT>(dest, ireg);
}
