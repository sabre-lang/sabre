/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Typed, node, analyzer) { analyzer->visit(node->value()); }
SABRE_MM_CHECK_NODE(Typed, node, analyzer) {
  $_UNUSED $_AUTO = analyzer->trace(node);
  auto result = analyzer->check(node->value());
  if (result.type->is<Type::Any>()) return result;
  return analyzer->instantiate(result.type, node->types());
}
