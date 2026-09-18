/// Builtins Includes
#include "sabre/builtins/_inline/builtins.ipp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

Sabre::Type::Entity Sabre::Type::Instance::lookup(const $::String::View &field) const {
  return m_prototype ? m_prototype->m_lookup(field) : Entity(New::any());
}

Sabre::Type::Erased Sabre::Type::Instance::apply(Operator::Kind kind) const { return apply(kind, nullptr); }
Sabre::Type::Erased Sabre::Type::Instance::apply(Operator::Kind kind, const Erased &right) const {
  if (m_prototype == nullptr) return New::any();
  return m_prototype->m_structure->apply(kind, right);
}

//  PRIVATE METHODS  //

Sabre::Type::Erased Sabre::Type::Instance::m_infer(Constraints *constraints) const {
  if (m_prototype == nullptr) return $::Shared::New<Instance>(*this);
  return m_prototype->infer(constraints)->as<Prototype>()->instantiate();
}

bool Sabre::Type::Instance::m_unify(const Erased &candidate, Constraints *constraints) const {
  // attempt resolving a suitable prototype
  auto other = New::cast<Prototype>(candidate, false);

  if (other == nullptr) return false;      // failed to find a parent class
  if (m_prototype == nullptr) return true; // pass all generic objects now

  // otherwise we check against the incoming class
  return m_prototype->unify(other, constraints);
}

void Sabre::Type::Instance::m_print(std::ostream &os, const Instance &self) {
  // do a regular print if there is no prototype available
  if (!self.m_prototype) return void(os << Builtins::Inspect<Object::Instance>::name());

  // otherwise we want to print the base-class name
  os << self.m_prototype->name();
  if (self.m_prototype->constraints().empty()) return;

  // and bind the incoming constraints now as necessary
  os << fmt::format("[{0}]", fmt::join($::Ranges::Deref(self.m_prototype->constraints()), ", "));
}
