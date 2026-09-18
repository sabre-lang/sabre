/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Panic, node, compiler, ) {
  $_UNUSED $_AUTO = compiler->trace(node);
  compiler->panic(node->value()); // bind
}
