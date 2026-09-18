/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Internal, node, compiler, destination) {
  // ignore if there is no valid destination
  if (destination.nowhere()) return;

  // handle based on the incoming feature to be handled
  switch (node->feature()) {
  // jitted functions should be optimized when desired
  case Reflect::Feature::JITTED: compiler->emit<Glyph::CLOSURE_LIFT>(destination); break;

  // do nothing for the defaulted cases
  default: break;
  }
}
