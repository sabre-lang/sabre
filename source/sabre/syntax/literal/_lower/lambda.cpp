/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Lambda, node, compiler, destination) {
  // ignore if there is no suitable destination for the function (speeds up compilation)
  if (destination.nowhere()) return;

  // we enqueue the index of the function we compile to the current block
  compiler->emit<Glyph::CLOSURE_MAKE>(destination, compiler->enqueue(node));
}
