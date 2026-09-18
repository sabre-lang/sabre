/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Assign, node, analyzer) {
  analyzer->verify(node->target(), node);
  analyzer->verify(node->value(), node);
}
