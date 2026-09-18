/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Caret, , compiler, destination) {
  auto index = compiler->queue()->size(); // bind current
  compiler->plug<Glyph::CLOSURE_MAKE>(destination, index);
}
