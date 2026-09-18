/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Overload, node, analyzer) { analyzer->verify(node->signature(), node); }
