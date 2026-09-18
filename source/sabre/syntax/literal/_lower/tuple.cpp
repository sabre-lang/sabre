/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Tuple, node, compiler, destination) {
  // trace the incoming binary node now
  $_UNUSED $_AUTO = compiler->trace(node);

  // if empty, then construct an empty instance
  if (node->empty()) return compiler->plug<Glyph::LIST_EMPTY>(destination);

  // prepare a suitable arguments list
  auto arguments = compiler->registers()->list();

  // construct the list to be used now
  for (const auto &element : node->elements()) compiler->lower(element, arguments.grow());

  // finally construct the list instance now
  compiler->plug<Glyph::LIST_MAKE>(destination, arguments);
}
