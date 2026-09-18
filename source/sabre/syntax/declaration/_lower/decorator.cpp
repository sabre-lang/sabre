/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Decorator, node, compiler, destination) {
  // ensure the incoming destination is valid
  if (destination.nowhere()) return;

  // prepare a suitable list for compilation
  auto list = compiler->registers()->list();

  // prepare the decorator to be called and the target to bind
  compiler->lower(node->expression(), list.grow());
  compiler->emit<Glyph::REG_MOVE>(list.grow(), destination);

  // attempt applying the node now to the built arguments
  compiler->emit<Glyph::CALL_N_VOID>(destination, list);
}
