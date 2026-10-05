/// Type Includes
#include "sabre/type/_inline/type.ipp"

//  OPERATOR METHODS  //

Sabre::Type::Erased
Sabre::Type::Utility::Loopable::operator()(const Erased &target, Constraints *constraints) const noexcept {
  auto inferred = target->infer(constraints);
  auto value = inferred->apply(Operator::Kind::ITER);
  if (value->is<Unset>()) return New::never();
  return m_outer ? New::iterator(value) : value;
}
