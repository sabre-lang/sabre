/// Builtins Includes
#include "sabre/builtins/_inline/builtins.ipp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

//  PRIVATE METHODS  //

Sabre::Type::Entity Sabre::Type::Callable::m_lookup(const $::String::View &field) const {
  // prepare a global function instance to get field-types from
  static auto s_instance = Builtins::Wrapper<Function::Any>::typeclass()->instantiate();

  // return the field that we can determine
  return s_instance->lookup(field);
}

Sabre::Type::Erased Sabre::Type::Callable::m_infer(Constraints *constraints) const {
  // prepare the cloned callable instance
  $::Shared::Pointer<Callable> callable = packed() ? $::Shared::New<Variadic>(*this) : $::Shared::New<Callable>(*this);

  // ignore if not given any generics
  if (constraints == nullptr) return callable;

  // otherwise rebuild the parameters and return value
  callable->returns() = m_returns->infer(constraints);

  // instantiate all the parameters
  for (const auto &[ii, p] : $::Ranges::Each(m_parameters)) callable->m_parameters[ii] = p.infer(constraints);

  // and return the resulting callable instance now
  return callable;
}

bool Sabre::Type::Callable::m_unify(const Erased &candidate, Constraints *constraints) const {
  auto other = New::cast<Callable>(candidate);
  if (other == nullptr) return false; // failed

  // ensure we validate our size of parameters before continuing
  if (arity() > other->arity() || adicity() < other->adicity()) return false;

  auto maximum = m_parameters.size(); // prepare details
  auto spread = packed() ? m_parameters.back() : Entity();

  // stop if any of the parameters are invalid
  for (const auto &[ii, right] : $::Ranges::Each(other->m_parameters)) {
    const auto &left = ii < maximum ? m_parameters.at(ii) : spread;
    $_ASSERT(left.opaque() && right.opaque(), "Parameters should be opaque");
    if (!left.value()->unify(right.value(), constraints)) return false;
  }

  // finally ensures the return value is also the same
  return m_returns->unify(other->m_returns, constraints);
}

void Sabre::Type::Callable::m_print(std::ostream &os, const Callable &self) {
  // prepare the transformation predicate to be used
  constexpr auto s_predicate = [](const Entity &entity) {
    return fmt::format("{0}", *entity.value()) + (entity.optional() ? "?" : "");
  };

  // build our parameters to be printed as necessary
  auto parameters = $::Ranges::To(self.m_parameters | std::views::transform(s_predicate));

  // prepare the packed details to be used now
  auto spread = self.packed() ? self.m_parameters.back().value() : nullptr;
  if (spread != nullptr) parameters.pop_back(); // pop the spread typing now

  // get the current emptiness to be used
  auto empty = parameters.empty();
  auto parens = !empty || spread;

  // test for the "Any" function instance
  if (spread && empty && spread->is<Any>() && self.m_returns->is<Any>()) {
    os << Builtins::Inspect<Function::Any>::name();
  } else {
    os << fmt::format("fn{0}{1}", parens ? " (" : "", fmt::join(parameters, ", "));
    if (spread) os << (empty ? "" : ", ") << "..." << *spread;
    os << (parens ? ")" : "") << " -> " << *self.m_returns;
  }
}
