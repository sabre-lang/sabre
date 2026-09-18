/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Override, node, analyzer) { analyzer->verify(node->target(), node); }
