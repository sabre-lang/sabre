/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Deprecated, , ) {}

SABRE_MM_CHECK_NODE(Deprecated, node, analyzer) {
  // get the available world instance
  auto *world = analyzer->world();

  // update the current deprecation details
  auto *entity = world->preamble();
  entity->deprecated(node->message());

  // and declare as passable now
  return analyzer->passable();
}
