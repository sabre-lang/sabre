/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Header, , , ) { /** headers are type-system only */ }
SABRE_MM_LOWER_NODE(Class, node, compiler, ) {
  // prepare the trace for the node now
  $_UNUSED $_AUTO = compiler->trace(node);

  // get the location of the variable now
  auto [dest, leaked] = compiler->declare(node);
  $_ASSERT(!dest.nowhere(), "Declaration does not exist");

  // prepare some details about the class
  auto treg = leaked ? Register::Accumulator : dest;

  // enqueue the underlying constructor
  auto constructor = compiler->enqueue(node);

  // prepare the baseline class instance to be used
  auto name = compiler->string(node->name());
  auto shape = compiler->shapes()->resolve(node);

  // prepare the baseline register now as well
  if (auto *base = node->base()) compiler->lower(base, treg);
  else compiler->emit<Glyph::LOAD_VOID>(treg); // default

  // enqueue the class for compilation now
  compiler->emit<Glyph::CLASS_MAKE>(treg, name, shape);
  compiler->emit<Glyph::CLASS_BIND>(treg, constructor);

  // we want to update the current value details
  compiler->preamble(node, treg);

  // and handle assignment based on the leakage state
  if (leaked) compiler->emit<Glyph::STORE_CONTEXT>(dest, treg);
}
