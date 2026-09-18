/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Infer, node, analyzer) {
  analyzer->visit(node->value());
  analyzer->visit(node->guard());
}

SABRE_MM_CHECK_NODE(Infer, node, analyzer) {
  // prepare the underlying trace to be completed
  $_UNUSED $_AUTO = analyzer->trace(node);

  // first start by analyzing each incoming instance
  auto value = analyzer->check(node->value()).type;
  auto guard = analyzer->check(node->guard()).type;

  // prepare the output typing to allow valid results
  auto result = Type::New::boolean();
  auto expecting = !node->negate();

  // stop whenever the value is a bad typing
  if (value->is<Type::Poison>()) return analyzer->passable(result);

  // guards that are "Any" should be noted as being redundant
  if (guard->is<Type::Any>()) analyzer->report(3000361, expecting ? "succeed" : "fail");

  // otherwise check if the guard will always fail now
  else if (guard->unify(value) != expecting) analyzer->report(expecting ? 3000362 : 3000363, *value, *guard, "fail");

  // should be a suitably valid type-cast to conclude
  return analyzer->passable(result);
}
