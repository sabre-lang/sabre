/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Tuple, node, analyzer) { analyzer->verify(node->elements(), node); }
