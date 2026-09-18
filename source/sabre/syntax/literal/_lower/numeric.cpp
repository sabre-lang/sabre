/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"
#include "sabre/number/tagged.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Numeric, node, compiler, destination) {
  // ignore if there is no sink to output to
  if (destination.nowhere()) return;

  // prepare the value to be used now
  auto value = node->value();

  // handle consistent values now
  if (value == 0) return compiler->emit<Glyph::LOAD_ZERO>(destination);
  else if (value == 1) return compiler->emit<Glyph::LOAD_ONE>(destination);

  // prepare the constant to be used now
  auto constant = compiler->constant(Number::Tagged(value));
  compiler->emit<Glyph::LOAD_CONST>(destination, constant);
}
