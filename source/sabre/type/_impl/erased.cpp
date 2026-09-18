/// Builtins Includes
#include "sabre/builtins/_inline/builtins.ipp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

Sabre::Type::Entity Sabre::Type::Unit::lookup(const $::String::View &) const { return Entity(); }

Sabre::Type::Erased Sabre::Type::Unit::apply(Operator::Kind kind) const {
  switch (kind) {
  case Operator::Kind::NOT: return New::boolean();
  default: return New::unset(); // unknown operator
  }
}

Sabre::Type::Erased Sabre::Type::Unit::apply(Operator::Kind kind, const Erased &right) const {
  switch (kind) {
  // prepare the kinds to be used now
  case Operator::Kind::EQ: $_FALLTHROUGH;
  case Operator::Kind::NE: $_FALLTHROUGH;
  case Operator::Kind::LOR: $_FALLTHROUGH;
  case Operator::Kind::LAND: return New::boolean();

  // assignments must be valid unifications
  case Operator::Kind::ASGN: return unify(right) ? right : New::unset();

  // nullish coalescing allows us to return ourselves or the right-most
  case Operator::Kind::COAL: return New::coalesce(m_self(), right);

  // should default to be an unknown value here
  default: return New::unset();
  }
}

Sabre::Type::Erased Sabre::Type::Unit::infer(Constraints *constraints) const { return m_infer(constraints); }

bool Sabre::Type::Unit::unify(const Erased &candidate, Constraints *constraints) const {
  // handle some immediate conditions that may occur
  if (this == candidate.get()) return true;

  // check against parameters and generics now
  if (candidate->is<Parameter>()) return m_solve(candidate->as<Parameter>().get(), constraints);
  if (candidate->is<Generic>() && !is<Generic>()) return m_solve(candidate->as<Generic>().get(), constraints);

  // pre-check for incoming never-types
  auto never = candidate->is<Union>() && candidate->as<Union>()->never();

  // bypass simple conditions (such as "Any" and "Never" types)
  if (!is<Parameter>() && (candidate->is<Any>() || never)) return true;

  // otherwise try the abstracted unification handler
  return m_unify(candidate, constraints);
}

//  PRIVATE METHODS  //

Sabre::Type::Erased Sabre::Type::Unit::m_infer(Constraints *) const { return m_self(); }

bool Sabre::Type::Unit::m_solve(const Generic *generic, Constraints *constraints) const {
  auto fallback = Constraints(); // bind fallback here
  if (constraints == nullptr) constraints = &fallback;

  // prepare the distinct typings to be used now
  auto distinct = std::vector<Erased>();

  // emplace all the generic parameters to be inferred
  for (const auto &parameter : generic->parameters()) {
    auto constraint = $::Shared::New<Parameter>(*parameter); // clone the constraint
    distinct.emplace_back(constraint), (*constraints)[constraint.get()] = New::unset();
  }

  // and finally attempt unification with the distinct parameters now
  return m_unify(generic->instantiate(distinct), constraints);
}

bool Sabre::Type::Unit::m_solve(const Parameter *parameter, Constraints *constraints) const {
  // ignore if there are no constraints available
  if (constraints == nullptr) return m_unify(parameter->clone(), constraints);

  auto iter = constraints->find(parameter); // check for bound, and allow binding now
  if (iter == constraints->cend()) return m_unify(parameter->clone(), constraints);

  // check if the parameter is being inferred, so clone the type now
  if (iter->second->is<Unset>()) return (*constraints)[parameter] = m_self(), true;

  // otherwise we check for a suitably match now
  return iter->second.get() == this || m_unify(iter->second, constraints);
}

void Sabre::Type::Unit::m_print(std::ostream &os, const Unit &self) {
#define X(T, ...)                                        \
  case $::RTTI::Hash<T>(): os << *self.m_as<T>(); break;
  switch (self.m_hash()) {
    X(Any)
    X(None)
    X(Unset)
    X(Poison)

    X(Union)
    X(Generic)
    X(Parameter)
    X(Transform)

    X(Callable)
    X(Instance)
    X(Structure)
    X(Prototype)

  // ensure we have a safe default since should never occur
  default: $_ABORT("Unknown Type::Unit '{0}'", self.m_hash()); break;
  }
#undef X
}
