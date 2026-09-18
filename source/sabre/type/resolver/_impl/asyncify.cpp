/// Type Includes
#include "sabre/type/_inline/type.ipp"

//  OPERATOR METHODS  //

Sabre::Type::Erased
Sabre::Type::Utility::Asyncify::operator()(const Erased &target, Constraints *constraints) const noexcept {
  // attempt instantiating the incoming target
  auto instantiated = target->infer(constraints);

  // resolve the incoming callback to be used
  auto callable = New::cast<Callable>(instantiated);

  // update the return typing now
  auto awaited = New::awaited(callable->returns());
  callable->returns() = New::future(awaited);

  // and return the instantiated instance (in case of generics)
  return instantiated;
}

//  PRIVATE METHODS  //

Sabre::Type::Erased
Sabre::Type::Utility::Asyncify::m_resolve(const Erased &target, Constraints *constraints) const noexcept {
  // prepare the future instance to be used now
  static auto s_future = New::future(New::any());

  // instantiate the underlying target typing
  auto instantiated = target->infer(constraints);

  // stop when we do not have a future-based typing or is a never typing
  if (instantiated->is<Any>() || !s_future->unify(instantiated)) return instantiated;
  else if (instantiated->is<Union>() && instantiated->as<Union>()->never()) return instantiated;

  auto prototype = instantiated->as<Instance>()->prototype();
  return m_resolve(prototype->constraints().at(0), constraints);
}
