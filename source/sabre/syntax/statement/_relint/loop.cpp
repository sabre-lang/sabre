/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(For, node, analyzer) {
  $_UNUSED $_AUTO = analyzer->scope();
  analyzer->verify(node->iterable(), node);
  analyzer->verify(node->statement(), node);
  analyzer->verify(node->parameters(), node);
}

SABRE_MM_LINT_NODE(Loop, node, analyzer) {
  $_UNUSED $_AUTO = analyzer->scope();
  analyzer->verify(node->condition(), node);
  analyzer->verify(node->statement(), node);
}
