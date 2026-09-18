/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Signature, node, analyzer) {
  $_UNUSED $_AUTO = analyzer->scope();
  analyzer->visit(node->prototype());
  analyzer->visit(node->returns());
}

SABRE_MM_CHECK_NODE(Signature, node, analyzer) {
  // force an immediate scoping to occur now
  $_UNUSED $_AUTO = analyzer->scope();
  $_UNUSED $_AUTO = analyzer->trace(node);

  // check the underlying signature typing first
  auto signature = analyzer->check(node->prototype()).type;
  auto *parameters = node->prototype()->parameters(); // update retval
  auto callable = parameters->trivia()->type()->as<Type::Callable>();
  callable->returns() = analyzer->check(node->returns()).type;

  // and return the resulting signature typing as necessary
  return signature;
}
