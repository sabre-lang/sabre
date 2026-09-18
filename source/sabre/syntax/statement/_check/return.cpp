/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Return, node, analyzer) { analyzer->visit(node->value()); }
SABRE_MM_CHECK_NODE(Return, node, analyzer) {
  // push the necessary traces for analyzing
  $_UNUSED $_AUTO = analyzer->trace(node);

  // get the expected return typing now
  Type::Erased callee = analyzer->world()->callee();

  // handle if we are not in a valid context
  if (callee == nullptr) return analyzer->report(3000900);

  // resolve the callable instance now if we have a generic
  auto callable = Type::New::cast<Type::Callable>(callee);

  // ensure the expected/inferred typings are valid
  auto expected = callable->returns(), inferred = analyzer->check(node->value()).type;

  // ensure the incoming return value is valid
  if (!expected->unify(inferred)) analyzer->report(3000300, *inferred, *expected);

  // always declare as currently unreachable now
  return analyzer->unreachable();
}
