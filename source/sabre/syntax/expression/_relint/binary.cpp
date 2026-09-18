/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Binary, node, analyzer) {
  analyzer->verify(node->left(), node);
  analyzer->verify(node->right(), node);
}
