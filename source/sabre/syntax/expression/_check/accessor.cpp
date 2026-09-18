/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Accessor, node, analyzer) { analyzer->visit(node->parent()); }
SABRE_MM_CHECK_NODE(Accessor, node, analyzer) {
  $_UNUSED $_AUTO = analyzer->trace(node);
  auto parent = analyzer->check(node->parent());

  // if any, the return the any result immediately
  if (parent.type->is<Type::Any>()) return parent;

  // attempt looking up the desired field
  auto field = node->field()->name();
  auto entity = parent.type->lookup(field);

  // ensure the entity is actually a valid value
  if (entity.unset()) return analyzer->report(4000200, field, *parent.type);
  if (!entity.opaque()) return analyzer->report(3000200, field);

  // if the entity is deprecated then declare as such
  analyzer->deprecated(&entity, node);

  // check if the incoming entity is private / protected at all
  if (entity.modifiers().test(Variable::Flag::PRIVATE, Variable::Flag::PROTECTED)) {
    // get the current self value to be used now
    auto *self = analyzer->world()->lookup("Self");

    if (self == nullptr) return analyzer->report(4000201, field); // not publicly accessible
    if (self->value() != parent.type) return analyzer->report(4000200, field, *parent.type);
  }

  // and finally declare the entity as valid now
  return node->field()->trivia()->type() = entity.value();
}
