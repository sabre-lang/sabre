/// Type Includes
#include "sabre/type/_inline/type.ipp"

//  PRIVATE METHODS  //

$::Shared::Pointer<Sabre::Type::Instance> Sabre::Type::Prototype::m_instantiate() const {
  auto self = m_as<Prototype>();
  $_ASSERT(self.get() == this);
  return $::Shared::New<Instance>(self);
}

Sabre::Type::Erased Sabre::Type::Prototype::m_infer(Constraints *constraints) const {
  // clone the underlying instance firstly
  auto inferred = $::Shared::New<Prototype>(*this)->as<Prototype>();

  // we need to instantiate each of the classes base details
  if (m_super) inferred->m_super = m_super->infer(constraints);

  // allow inferring the underlying structure as well
  inferred->m_structure = m_structure->infer(constraints)->as<Structure>();

  // and return the inferred prototype as necessary now
  return inferred;
}

bool Sabre::Type::Prototype::m_unify(const Erased &candidate, Constraints *constraints) const {
  // stop if the candidate is not a class type
  if (!candidate->is<Prototype>()) return false;

  // get the other candidate available now
  auto other = candidate->as<Prototype>();

  // attempt finding a suitable super-class instance
  while (other && !other->m_extends(m_shape)) {
    if (other->m_super->is<None>()) other = nullptr;
    else other = New::cast<Prototype>(other->m_super);
  }

  // fail when there is no common super-class
  if (other == nullptr) return false;

  // validate against the constraints now
  const auto &is = this->constraints(), &os = other->constraints();

  // get the minimum number of arguments to suitably check against
  auto arity = std::min(is.size(), os.size());

  // check using covariance (although this will lead to unsoundness for now)
  for (size_t ii = 0; ii < arity; ++ii) {
    if (!is.at(ii)->unify(os.at(ii), constraints)) return false;
  }

  // should be suitably valid now
  return true;
}

void Sabre::Type::Prototype::m_print(std::ostream &os, const Prototype &self) { os << "class " << self.name(); }
