/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Variable, node, analyzer) {
  analyzer->bind(node);
  analyzer->preamble(node);
  analyzer->visit(node->hint());
  analyzer->visit(node->initializer());
}

SABRE_MM_CHECK_NODE(Variable, node, analyzer) {
  // prepare the base variable report to be used
  $_UNUSED $_AUTO = analyzer->trace(node);

  // get the incoming expected typing
  auto [name, expected] = analyzer->vardef(node);

  // get the incoming modifiers to be checked against
  const auto &modifiers = node->modifiers();
  auto exported = modifiers.test(Variable::Flag::EXPORT);
  auto disposable = modifiers.test(Variable::Flag::DISPOSABLE);

  // ensure some conditions about the variable now
  if (exported && disposable) analyzer->report(3000150);

  // attempt declaring on the world now
  auto *entity = analyzer->world()->values().declare(analyzer->sanity(node), expected);
  if (entity == nullptr) return analyzer->report(4000403, name); // fail if necessary

  // update the entity if necessary to do so
  if (disposable) entity->unused(false);

  // handle all the preamble for the node
  return analyzer->preamble(node, entity);
}
