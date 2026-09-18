/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Alias, node, analyzer) {
  analyzer->visit(node->generics());
  analyzer->visit(node->hint());
}

SABRE_MM_CHECK_NODE(Alias, node, analyzer) {
  // prepare a lazy transform for our typing (for recursion)
  auto transform = Type::New::lazy();

  // prepare the details about the alias now
  auto name = node->name();
  auto bounds = node->trivia()->bounds();

  // sanity check the incoming node now
  analyzer->sanity(node);

  // pre-declare the entity as necessary
  auto *entity = analyzer->world()->types().declare(name, transform, bounds);
  if (entity == nullptr) return analyzer->report(node, 4000402, name);

  // prepare a current scoping to be used now
  $_UNUSED $_AUTO = analyzer->scope();

  // get the current generics to be used now
  auto constraints = analyzer->check(node->generics());

  auto hint = analyzer->check(node->hint()).type; // resolve the typing now
  if (hint->is<Type::Structure>()) hint->as<Type::Structure>()->name() = name;

  // update the entity to contain this hint now
  transform->target() = Type::New::generic(hint, constraints);

  // otherwise update the exported flag if necessary
  auto exported = node->modifiers().test(Variable::Flag::EXPORT);
  if (exported) entity->modifiers().set(Variable::Flag::EXPORT);

  // return a passable result now
  return analyzer->passable(transform);
}
