/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Panic, node, analyzer) { analyzer->verify(node->value(), node); }
