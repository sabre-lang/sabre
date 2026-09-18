/// Type Includes
#include "sabre/type/_inline/type.ipp"

//  OPERATOR METHODS  //

Sabre::Type::Erased
Sabre::Type::Utility::Awaited::operator()(const Erased &target, Constraints *constraints) const noexcept {
  return m_resolve(target, constraints);
}

//  PRIVATE METHODS  //

Sabre::Type::Erased
Sabre::Type::Utility::Awaited::m_resolve(const Erased &target, Constraints *constraints) const noexcept {
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
