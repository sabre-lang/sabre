/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Group, node, compiler, destination) { compiler->lower(node->value(), destination); }
