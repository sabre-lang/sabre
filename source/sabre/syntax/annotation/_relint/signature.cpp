/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Parameters, node, analyzer) { analyzer->verify(node->list(), node); }

SABRE_MM_LINT_NODE(Constructor, node, analyzer) {
  analyzer->verify(node->generics(), node);
  analyzer->verify(node->parameters(), node);
}

SABRE_MM_LINT_NODE(Signature, node, analyzer) {
  $_UNUSED $_AUTO = analyzer->scope();
  analyzer->verify(node->prototype(), node);
  analyzer->verify(node->returns(), node);
}
