/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PRIVATE METHODS  //

Sabre::Type::Erased Sabre::Type::Parameter::m_infer(Constraints *constraints) const {
  return constraints && constraints->contains(this) ? constraints->at(this) : clone();
}

bool Sabre::Type::Parameter::m_unify(const Erased &candidate, Constraints *constraints) const {
  // ignore if there are no constraints available
  if (constraints == nullptr) return m_extends->unify(candidate);

  // check if this parameter exist in the available constraints
  auto iter = constraints->find(this);
  if (iter == constraints->cend()) return false;

  // check if the parameter is being inferred, so clone the type now
  if (iter->second->is<Unset>()) {
    if (!m_extends->unify(candidate)) return false;
    return (*constraints)[this] = candidate, true;
  }

  // otherwise we check for a suitable match now
  return iter->second.get() == this || iter->second->unify(candidate, constraints);
}
