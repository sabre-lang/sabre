/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Accessor, node, compiler, destination) {
  // ensure we load the parent into the accumulator (side-effects)
  compiler->lower(node->parent(), destination);

  // ignore loading the field if the destination is nowhere
  if (destination.nowhere()) return;

  // prepare the trace for the node now
  $_UNUSED $_AUTO = compiler->trace(node);

  // attempt loading the field as necessary now
  auto symbol = compiler->symbol(node->field()->name());
  compiler->emit<Glyph::LOAD_FIELD>(destination, destination, symbol);
}
