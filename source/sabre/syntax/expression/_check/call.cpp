/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Declarations
$_FWD(Sabre::Type::Dispatch, Erased infer(Generic *, const Constraints *))
$_FWD(Sabre::Type::Dispatch, Erased bind(Parameter *, const Constraints *))

//  PUBLIC METHODS  //

Sabre::Type::Erased Sabre::Type::Dispatch::bind(Parameter *parameter, const Constraints *constraints) {
  auto bound = constraints->find(parameter); // attempt finding the suitable bound typing now
  $_ASSERT(bound != constraints->cend(), "Generic parameter '{0}' is missing", parameter->name());

  // check if we do not have a futher generic parameter to be inferred
  if (!bound->second->is<Parameter>()) return bound->second;

  // and since it is, we need to resolve further now
  auto *nested = bound->second->as<Parameter>().get();
  if (nested == parameter) return bound->second;
  if (!constraints->contains(nested)) return bound->second;

  // otherwise attempt resolving the nested instance now
  return bind(nested, constraints);
}

Sabre::Type::Erased Sabre::Type::Dispatch::infer(Generic *generic, const Constraints *constraints) {
  // prepare the baseline outputs now
  auto &parameters = generic->parameters();
  auto bound = std::vector<Erased>(parameters.size());

  // attempt modifying all the items now
  for (const auto &[ii, parameter] : $::Ranges::Each(parameters)) bound[ii] = bind(parameter.get(), constraints);

  // and instantiate the resulting generics now
  return generic->instantiate(bound);
}

SABRE_MM_CAPTURE_NODE(Call, node, analyzer) {
  analyzer->visit(node->callee());
  analyzer->visit(node->arguments());
}

SABRE_MM_CHECK_NODE(Call, node, analyzer) {
  // attempt getting the callee typing
  auto callee = analyzer->check(node->callee()).type;

  // allow any-types to immediately be passed through
  if (callee->is<Type::Any>()) return analyzer->check(node->arguments()), callee;

  // resolve the underlying details for the callee
  auto generic = Type::New::cast<Type::Generic>(callee);
  auto callable = Type::New::cast<Type::Callable>(callee);

  // stop if the callable does not actually exist
  if (callable == nullptr) return analyzer->report(node->callee(), 3000550, *callee);

  // prepare some common items to be filled
  auto arguments = std::vector<Type::Erased>();
  auto constraints = Type::Constraints();

  // prepare some base details about the initial callable instance
  auto parameters = callable->parameters();
  auto total = node->arguments().size(), maximum = parameters.size();
  auto spread = callable->packed() ? parameters.back() : Type::Entity();

  // fill the generics cache to be used now
  for (const auto &constraint : generic ? generic->parameters() : Type::Template()) {
    constraints.emplace(constraint.get(), Type::New::unset()); // push unset type
  }

  // pre-construct the incoming arguments to be handled now
  for (const auto &[ii, argument] : $::Ranges::Each(node->arguments())) {
    auto type = analyzer->check(argument).type; // analyze the argument now
    auto parameter = ii < maximum ? callable->parameters().at(ii) : spread;
    if (parameter.value()->unify(type, &constraints)) arguments.emplace_back(type);
    else analyzer->report(argument, 3000301, *type, *parameter.value());
  }

  // if we did not validaly construct all our arguments, then forcibly fail
  if (arguments.size() != total) return Type::New::fail();

  // pre-infer types if necessary to do so
  if (generic) {
    auto resolved = Type::Dispatch::infer(generic.get(), &constraints);
    if (resolved->is<Type::Any>()) return analyzer->passable(Type::New::any());
    if (auto inferred = Type::New::cast<Type::Callable>(resolved)) callable = inferred;
    else return analyzer->report(node->callee(), 3000552); // otherwise failed to resolve

    // update the underlying callee typing now
    node->callee()->trivia()->type() = callable;
  }

  // resolve the parameters and details a second time
  parameters = callable->parameters(), maximum = parameters.size();
  spread = callable->packed() ? parameters.back() : Type::Entity();

  // and get the incoming arity/adicity details to be used
  auto arity = callable->arity(), adicity = callable->adicity();
  auto code = arity != adicity ? 3000501 : 3000502; // prepare code

  // ensure the total is suitably valid
  if (total < arity || total > adicity) return analyzer->report(node->callee(), code, arity, total);

  // ensure all our arguments have been type-checked now
  for (const auto &[ii, argument] : $::Ranges::Each(node->arguments())) {
    // ensure we check the incoming argument typing
    auto type = arguments.at(ii);

    // ignore if the type-parameter resolution is not found
    if (ii >= adicity) continue;
    if (ii >= maximum && !spread.unset()) continue;

    // get the current parameter in question
    auto parameter = ii < maximum ? parameters.at(ii) : spread;

    // otherwise type-check the parameter now
    if (parameter.optional() && type->is<Type::None>()) continue;
    else if (parameter.value()->unify(type, &constraints)) continue;
    analyzer->report(argument, 3000301, *type, *parameter.value());
  }

  // resolve the final callable instance now
  return analyzer->passable(callable->returns());
}
