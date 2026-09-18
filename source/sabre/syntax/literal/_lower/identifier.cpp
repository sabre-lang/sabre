/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Self, , compiler, destination) { compiler->plug<Glyph::LOAD_SELF>(destination); }

SABRE_MM_LOWER_NODE(Identifier, node, compiler, destination) {
  $_UNUSED $_AUTO = compiler->trace(node);
  auto extent = compiler->captures()->extent(node);
  compiler->load(node->name(), extent, destination);
}
