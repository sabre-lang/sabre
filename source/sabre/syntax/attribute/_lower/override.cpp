/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Override, node, compiler, destination) {
  // ensure the incoming destination is valid
  if (destination.nowhere()) return;

  // prepare a temporary register for the incoming value
  auto treg = compiler->registers()->temporary();

  // lower the incoming target to be updated
  compiler->lower(node->target(), treg);

  // convert the operator kind into an operand
  auto kind = static_cast<Bytecode::Index::Encoded>(node->kind());

  // assign the operator as necessary now
  compiler->emit<Glyph::OBJECT_ATTR>(destination, treg, kind);
}
