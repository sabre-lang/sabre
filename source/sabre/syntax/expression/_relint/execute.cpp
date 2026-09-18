/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Execute, node, analyzer) { analyzer->verify(node->invocation(), node); }
