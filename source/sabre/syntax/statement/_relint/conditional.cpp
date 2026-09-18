/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Conditional, node, analyzer) {
  analyzer->verify(node->condition(), node);
  analyzer->verify(node->consequence(), node);
  analyzer->verify(node->alternative(), node);
}
