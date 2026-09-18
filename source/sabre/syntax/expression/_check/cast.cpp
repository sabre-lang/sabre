/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Cast, node, analyzer) {
  analyzer->visit(node->value());
  analyzer->visit(node->guard());
}

SABRE_MM_CHECK_NODE(Cast, node, analyzer) {
  // prepare the underlying trace to be completed
  $_UNUSED $_AUTO = analyzer->trace(node);

  // first start by analyzing each incoming instance
  auto value = analyzer->check(node->value()).type;
  auto guard = analyzer->check(node->guard()).type;

  // stop whenever the value is a bad typing
  if (value->is<Type::Poison>()) return analyzer->passable(guard);

  // guards that are declared as any can be immediately ignored
  if (guard->is<Type::Any>()) return analyzer->passable(guard);

  if (value->is<Type::Any>()) analyzer->report(3000360); // ensure convertible now
  else if (!guard->unify(value)) return analyzer->report(3000352, *value, *guard);

  // declare as suitably convertible now
  return analyzer->passable(guard);
}
