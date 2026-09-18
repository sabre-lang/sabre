/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Unary, node, analyzer) { analyzer->verify(node->operand(), node); }
