/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Tuple, node, analyzer) { analyzer->visit(node->elements()); }
SABRE_MM_CHECK_NODE(Tuple, node, analyzer) {
  // attempt reducing a suitable tuple value now
  Type::Erased inferred = Type::New::any();

  // attempt inferring the required tuple type here
  for (const auto &element : node->elements()) {
    auto result = analyzer->check(element); // get next now
    if (result.type->is<Type::Poison>()) return result;
    if (inferred->unify(result.type)) inferred = result.type;
    if (inferred->is<Type::Any>()) break; // suitably broken
  }

  // attempt resolving a suitable instance type for the list
  return Type::New::list(inferred);
}
