/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Declarations
$_FWD(Sabre::Type::Dispatch, Entity target(Analyzer *, const Syntax::Expression *))
$_FWD(Sabre::Type::Dispatch, Deduction immutable(Analyzer *, const Syntax::Expression *))

//  PUBLIC METHODS  //

Sabre::Type::Entity Sabre::Type::Dispatch::target(Analyzer *analyzer, const Syntax::Expression *target) {
  // handle identifiers by getting straight from the world
  switch (target->trivia()->hash()) {
  // identifiers can be found simply
  case $::RTTI::Hash<Syntax::Identifier>(): {
    auto *identifier = target->as<Syntax::Identifier>();
    auto *entity = analyzer->world()->lookup(identifier->name());
    return entity ? *entity : Entity(); // rebuild here if valid
  }

  // accessors need to be found elsewhere
  case $::RTTI::Hash<Syntax::Accessor>(): {
    // cast to the base accessor node
    auto *accessor = target->as<Syntax::Accessor>();

    // get the details about the accessor now
    auto field = accessor->field()->name();
    auto *parent = accessor->parent();

    // attempt a field lookup here now
    return parent->trivia()->type()->lookup(field);
  }

  // we have an invalid assignment target so fail immediately
  default: return Entity();
  }
}

Sabre::Type::Deduction Sabre::Type::Dispatch::immutable(Analyzer *analyzer, const Syntax::Expression *target) {
#define MM_AS(N) target->as<Syntax::N>()
  switch (target->trivia()->hash()) {
  case $::RTTI::Hash<Syntax::Identifier>(): return analyzer->report(3000100, MM_AS(Identifier)->name());
  case $::RTTI::Hash<Syntax::Accessor>(): return analyzer->report(3000102, MM_AS(Accessor)->field()->name());
  default: return analyzer->report(3000101); // we can suitably ignore this outgoing description
  }
#undef MM_AS
}

SABRE_MM_CAPTURE_NODE(Assign, node, analyzer) {
  analyzer->visit(node->target());
  analyzer->visit(node->value());
}

SABRE_MM_CHECK_NODE(Assign, node, analyzer) {
  // prepare the underlying dispatch details
  $_UNUSED $_AUTO = analyzer->trace(node);

  // get the underlying target node now
  const auto *target = node->target();

  // pre-check the current assignment targets
  auto left = analyzer->check(target).type;
  auto right = analyzer->check(node->value()).type;

  // get the underlying assignment target now
  auto entity = Type::Dispatch::target(analyzer, target);
  if (!entity.opaque()) return analyzer->report(2000500);

  // report if we cannot mutate the desired entity instance
  if (entity.immutable()) Type::Dispatch::immutable(analyzer, target);

  // and return the final result now
  return left->unify(right) ? analyzer->passable(right) : analyzer->report(3000300, *right, *left);
}
