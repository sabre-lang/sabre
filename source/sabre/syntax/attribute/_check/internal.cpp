/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Internal, , ) {}

SABRE_MM_CHECK_NODE(Internal, node, analyzer) {
  // set the current trace handler
  $_UNUSED $_AUTO = analyzer->trace(node);

  // get the current preamble target to be validated
  auto target = analyzer->preamble()->value();

  // prepare a baseline callback to validate against
  auto callback = Type::New::variadic();

  // ensure that the target is actually valid now
  if (callback->unify(target)) return analyzer->passable();

  // otherwise need to show that the given feature is invalid
  auto feature = Reflect::Inspect::feature(node->feature());
  return analyzer->report(3000851, *target, feature);
}
