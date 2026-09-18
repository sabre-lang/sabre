/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Declarations
$_FWD(Sabre::Type::Dispatch, void member(Analyzer *, const Syntax::Declaration *, Prototype *))

//  PUBLIC METHODS  //

void Sabre::Type::Dispatch::member(Analyzer *analyzer, const Syntax::Declaration *field, Prototype *proto) {
  // get the underlying field details to be used
  auto type = analyzer->check(field).type;

  // attempt getting the current member now
  auto *entity = analyzer->world()->lookup(field->name());

  // update the prototype with the values details to be bound
  proto->fields().emplace(field->name(), *entity), entity->unused(false);

  // finally delete the incoming field name as well
  analyzer->world()->entities().erase(field->name());
}

SABRE_MM_CAPTURE_NODE(Header, node, analyzer) {
  analyzer->visit(node->constructor());
  analyzer->visit(node->super());
  analyzer->visit(node->implements());
}

SABRE_MM_CHECK_NODE(Header, node, analyzer) {
  analyzer->check(node->super(), Type::New::none());
  return analyzer->check(node->base(), Type::New::none());
}

SABRE_MM_CAPTURE_NODE(Class, node, analyzer) {
  $_UNUSED $_AUTO = analyzer->scope(node, analyzer->depth()); // prepare a class scoping
  analyzer->preamble(node), analyzer->visit(node->header()), analyzer->visit(node->fields());
}

SABRE_MM_CHECK_NODE(Class, node, analyzer) {
  // get the baseline range of the node
  auto range = node->trivia()->range();

  // pre-declare the entity instance now
  auto overlap = !analyzer->world()->declare(node->name(), range);
  if (overlap) return analyzer->report(4000406, node->name());

  // prepare the current scoping to be used
  $_UNUSED $_AUTO = analyzer->scope();

  // ensure that the signature is valid
  auto constructor = analyzer->check(node->constructor()).type;
  if (constructor->is<Type::Any>()) return constructor; // bail

  // attempt checking the baseline prototype details
  auto shape = analyzer->shapes()->assign(node);

  // construct the underlying class prototype now
  auto proto = Type::New::prototype(node->name(), shape);

  // and construct a baseline instance to be used now
  auto instance = proto->instantiate();

  // set some of the constructor details to be used
  auto generic = Type::New::cast<Type::Generic>(constructor);

  // pre-update the constructor with the instance as a return-type
  Type::New::cast<Type::Callable>(constructor)->returns() = instance;

  // update the necessary components to be used now
  auto parameters = generic ? generic->parameters() : Type::Template();
  proto->constraints() = $::Ranges::To<Type::Erased>(parameters);

  // update the underlying constructor function to be used
  proto->constructor() = [constructor, parameters](const Type::Prototype *proto) -> Type::Erased {
    // bind all the prototype constraints to
    auto constraints = Type::Constraints();

    // iterate over the avialable constraints to be bound
    for (const auto &[ii, parameter] : $::Ranges::Each(parameters)) {
      constraints.emplace(parameter.get(), proto->constraints().at(ii));
    }

    // use the prototype constraints to resolve the constructor
    return constructor->infer(&constraints);
  };

  // resolve the entity and it's context now
  auto *entity = analyzer->world()->lookup(node->name());

  // can safely update the underlying entity details now
  entity->modifiers() = node->modifiers();
  entity->value() = Type::New::generic(proto, parameters);
  entity->type() = Type::New::generic(instance, parameters);

  // update the current exported status as well
  if (entity->exported()) entity->unused(false);

  // prepare the current trace range
  $_UNUSED $_AUTO = analyzer->trace(node);

  // prepare the incoming world to be used for scoping
  auto callable = Type::New::cast<Type::Callable>(proto->callable());
  auto world = analyzer->scope(node->constructor(), callable, nullptr);

  // attempt resolving the incoming super typing
  auto base = analyzer->check(node->header(), Type::New::none()).type;
  if (base == proto) analyzer->report(node, 3001000, node->name()); // no self-references
  else if (auto _ = Type::New::cast<Type::Prototype>(base)) proto->super() = base;
  else if (!base->is<Type::None>()) analyzer->report(node->base(), 3001001, node->name(), *base);

  // ensure we declare the current outer shell now
  world->outer() = world->depth(), world->loops() = -1;

  // prepare the self and super value to be used now as well
  world->values().declare("Self", instance)->unused(false);

  // start attempting to declare all the available fields
  for (const auto &field : node->fields()) Type::Dispatch::member(analyzer, field, proto.get());

  // validate that the underlying "implements" values are valid
  for (const auto &implements : node->implements()) {
    auto interface = analyzer->check(implements).type;
    if (interface->unify(instance)) continue; // valid

    // declare that the incoming check failed
    analyzer->report(implements, 3001002, node->name(), *interface);

    /// TODO: we should be able to process further information here
  }

  // attempt checking all the available fields now for use
  return analyzer->preamble(node, entity);
}
