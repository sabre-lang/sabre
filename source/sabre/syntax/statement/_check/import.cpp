/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Import, node, analyzer) { analyzer->bind(node->patterns()); }
SABRE_MM_CHECK_NODE(Import, node, analyzer) {
  auto *module = analyzer->import(node->path(), node->trivia()->range());
  $_ASSERT(module, "Expected module to be a non-nullptr"); // ensure exists

  // prepare some details about the module
  auto *world = analyzer->world();
  auto exports = module->structure();
  auto dynamic = module->dynamic();
  auto wildcard = node->wildcard();

  // prepare the underlying dynamic type to be used
  auto monotype = dynamic ? module->monotype() : exports;

  // resolve the resource patterns to be used now
  for (const auto *pattern : node->patterns()) {
    // trace the pattern that we are resolving now
    $_UNUSED $_AUTO = analyzer->trace(pattern);

    // prepare some details about the pattern
    auto name = pattern->name();
    auto range = pattern->trivia()->range();

    // ensure we get the underlying entity to be used now
    auto exists = wildcard ? false : exports->fields().contains(name);
    auto entity = exists ? exports->lookup(name) : Type::Entity();

    // update the conditional details for the entity firstly
    if (node->exported()) entity.modifiers().set(Variable::Flag::EXPORT);
    else entity.modifiers().clear(Variable::Flag::EXPORT);

    // always update the entity details to be used
    entity.unused(!entity.exported());

    if (wildcard) entity.value() = monotype; // resolve the entity value
    else if (!exists && dynamic) exists = true, entity.value() = monotype;

    // declare an error for items that cannot be exported
    if (!exists && !wildcard) analyzer->report(8000300, name);
    else if (world->declare(name, entity, range)) continue;
    else analyzer->report(4000403, name); // failed to declare
  }

  // stop early if we had some patterns
  if (node->patterns().size() || !node->exported()) return analyzer->passable();

  // otherwise we need to check against barreling exports
  auto barrel = Type::New::cast<Type::Structure>(monotype);
  if (barrel == nullptr) return analyzer->passable();

  // iteratively expose all the barrel exports now
  for (const auto &[name, entity] : barrel->fields().view(barrel.get())) {
    auto success = world->declare(name, entity, node->trivia()->range());
    if (!success) analyzer->report(4000403, name); // failed to declare here
  }

  // always declare as passable (regardless) of errors
  return analyzer->passable();
}
