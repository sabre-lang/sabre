/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Declarations
$_FWD(Sabre::Type::Dispatch, Erased returns(const Syntax::Lambda *, const Erased &))
$_FWD(Sabre::Type::Dispatch, void lambda(Analyzer *, const Syntax::Lambda *, const Erased &))

//  PUBLIC METHODS  //

Sabre::Type::Erased Sabre::Type::Dispatch::returns(const Syntax::Lambda *lambda, const Erased &returns) {
  // return the original return value if valid
  if (!returns->is<Unset>()) return returns;

  // otherwise check if using an arrow function
  auto arrow = !lambda->body()->is<Syntax::Block>();

  // resolve as necesasry now
  return arrow ? New::any() : New::none();
}

void Sabre::Type::Dispatch::lambda(Analyzer *analyzer, const Syntax::Lambda *self, const Erased &callee) {
  // prepare the current trace location
  $_UNUSED $_AUTO = analyzer->trace(self);

  // prepare the callable instance
  auto generic = New::cast<Generic>(callee);
  auto callable = New::cast<Callable>(callee);

  // prepare the scoping instance
  auto *constructor = self->signature()->prototype();
  auto world = analyzer->scope(constructor, callable, generic);

  // update the current world callee to be used now
  world->callee() = callee;

  // attempt resolving the body now as normal
  auto result = analyzer->check(self->body());

  // push the return-typing as the current trace value
  $_UNUSED $_AUTO = analyzer->trace(self->signature()->returns());

  // ensure we use the implicit handler when necessary
  auto returns = callable->returns();

  // start determining the current return-typing
  auto arrow = !self->body()->is<Syntax::Block>();

  // ensure the flow is unreachable and non-arrow
  if (!arrow && result.flow->unreachable()) return;

  // attempt running a unification here as necessary
  if (returns->unify(result.type)) return;

  // otherwise handle the report that we need to give
  if (returns->is<Union>() && returns->as<Union>()->never()) analyzer->report(3000901);
  else if (!arrow && !returns->is<None>()) analyzer->report(3000902);
  else analyzer->report(self->body(), 3000300, *result.type, *returns);
}

SABRE_MM_CAPTURE_NODE(Lambda, node, analyzer) {
  analyzer->visit(node->signature());
  analyzer->defer([node](Analyzer *analyzer) {
    $_UNUSED $_AUTO = analyzer->scope(analyzer->depth());
    analyzer->bind(node->parameters()), analyzer->visit(node->body());
  });
}

SABRE_MM_CHECK_NODE(Lambda, node, analyzer) {
  // immediately ensure that the signature is valid
  auto result = analyzer->check(node->signature());

  // return early when necessary to do so here
  if (result.type->is<Type::Any>()) return result;

  // get the return-typing of the result
  auto callable = Type::New::cast<Type::Callable>(result.type);
  callable->returns() = Type::Dispatch::returns(node, callable->returns());

  // prepare the deferred check handler
  Type::Deferrer check = [node, type = result.type](auto *_) mutable { Type::Dispatch::lambda(_, node, type); };

  // push the incoming instance onto main checker now
  return analyzer->defer(std::move(result), std::move(check));
}
