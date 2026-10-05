/// Type Includes
#include "sabre/type/_inline/type.ipp"

//  OPERATOR METHODS  //

Sabre::Type::Erased
Sabre::Type::Utility::Disposable::operator()(const Erased &target, Constraints *constraints) const noexcept {
  // attempt instantiating the incoming target
  auto instantiated = target->infer(constraints);

  // resolve the incoming callback to be used
  auto callable = New::cast<Callable>(instantiated);

  // update the return typing now
  callable->returns() = New::disposable();

  // and return the instantiated instance (in case of generics)
  return instantiated;
}
