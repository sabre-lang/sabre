/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Text, node, compiler, destination) {
  if (destination.nowhere()) return; // ignore
  auto index = compiler->string(node->buffer());
  compiler->emit<Glyph::STRING_MAKE>(destination, index);
}
