/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Infer, node, analyzer) {
  analyzer->verify(node->value(), node);
  analyzer->verify(node->guard(), node);
}
