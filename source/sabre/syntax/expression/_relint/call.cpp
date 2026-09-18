/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Call, node, analyzer) {
  analyzer->verify(node->callee(), node);
  analyzer->verify(node->arguments(), node);
}
