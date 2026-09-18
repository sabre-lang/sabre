/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Record, node, compiler, destination) {
  // trace the incoming binary node now
  $_UNUSED $_AUTO = compiler->trace(node);

  // if empty, then construct an empty record
  if (node->empty()) return compiler->plug<Glyph::OBJECT_EMPTY>(destination);

  // prepare a suitable variables list
  auto arguments = compiler->registers()->list();

  // attempt building our values into a list
  for (const auto &element : node->elements()) {
    // ensure we trace each of the element declarations
    $_UNUSED $_AUTO = compiler->trace(element);

    // prepare the key-register and variable register
    auto kreg = arguments.grow(), vreg = arguments.grow();

    // bind the element to a symbol as well
    auto symbol = compiler->symbol(element->name());
    compiler->emit<Glyph::LOAD_CONST>(kreg, symbol);

    // prepare the initializer instance
    auto *initializer = element->initializer();
    if (initializer) compiler->lower(initializer, vreg);
    else compiler->emit<Glyph::LOAD_VOID>(vreg);
  }

  // once complete, construct an object literal
  compiler->plug<Glyph::OBJECT_MAKE>(destination, arguments);
}
