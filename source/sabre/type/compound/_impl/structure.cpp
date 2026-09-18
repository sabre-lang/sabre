/// Type Includes
#include "sabre/type/_inline/type.ipp"

//  PRIVATE METHODS  //

Sabre::Type::Erased Sabre::Type::Structure::m_infer(Constraints *constraints) const {
  // prepare an inferred instance now
  auto inferred = $::Shared::New<Structure>(*this);

  // infer all the constraints to be bound
  for (const auto &[ii, c] : $::Ranges::Each(m_constraints)) inferred->m_constraints[ii] = c->infer(constraints);

  // and return the resulting inferred instance now
  return inferred;
}

bool Sabre::Type::Structure::m_unify(const Erased &candidate, Constraints *constraints) const {
  // iterably ensure that every field has been fulfilled
  for (const auto &[name, entity] : m_fields.view(this, constraints)) {
    auto other = candidate->lookup(name); // get the other value now
    if (!other.value()->unify(entity.value(), constraints)) return false;
    if (other.modifiers().test(Variable::Flag::PRIVATE, Variable::Flag::PROTECTED)) return false;
  }

  // stop early if there is no fallback being used (successfully matched fields)
  if (m_fallback == nullptr) return true;

  // check basic shape is actually valid firstly (we expect only custom shapes)
  if (candidate->shape() < Shape::Lookup<Object::Instance>()) return false;

  /// TODO: finally we should be checking against every member value
  return m_fallback->is<Any>();
}

void Sabre::Type::Structure::m_print(std::ostream &os, const Structure &self) {
  // determine a suitably typing to be used now
  if (self.m_name.size()) os << self.m_name;
  else if (self.m_fallback == nullptr) os << "{...}";
  else if (self.m_fallback->is<Any>()) os << "Object";
  else os << "Record[" << *self.m_fallback << ']';

  // append the constraints used on the incoming object now
  if (self.m_constraints.size()) os << fmt::format("[{0}]", fmt::join($::Ranges::Deref(self.m_constraints), ", "));
}
