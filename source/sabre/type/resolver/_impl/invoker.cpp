/// Type Includes
#include "sabre/type/_inline/type.ipp"

//  OPERATOR METHODS  //

Sabre::Type::Erased
Sabre::Type::Utility::Invoker::operator()(const Erased &target, Constraints *constraints) const noexcept {
  // if we have no constraints, then we try binding the target typing
  if (constraints == nullptr) return m_bind(target);

  // attempt resolving the underlying instance now
  auto instantiated = target->infer(constraints);
  if (instantiated->is<Any>()) return New::any();
  if (!instantiated->is<Callable>()) return New::unset();

  // attempt converting the incoming function now
  auto callable = instantiated->as<Callable>();

  // prepare the return value and parameters
  auto returns = m_returns(callable);
  auto parameters = callable->parameters();

  // rebuild the parameters to include the callable itself
  parameters.emplace(parameters.begin(), callable);

  // and reconstruct the resulting callable instance now
  return callable->packed() ? New::variadic(returns, parameters) : New::function(returns, parameters);
}

//  PRIVATE METHODS  //

/**
 * @brief Handles binding function parameters.
 * @param target                Target to bind.
 */
Sabre::Type::Erased Sabre::Type::Utility::Invoker::m_bind(const Erased &target) const noexcept {
  return New::variadic(New::any(), target, New::any());
}

/**
 * @brief Handles resolving return-values.
 * @param callable              Callable to resolve.
 */
Sabre::Type::Erased
Sabre::Type::Utility::Invoker::m_returns(const $::Shared::Pointer<Callable> &callable) const noexcept {
  if (!m_async) return callable->returns(); // do not asyncify
  return New::future(New::awaited(callable->returns()));
}
