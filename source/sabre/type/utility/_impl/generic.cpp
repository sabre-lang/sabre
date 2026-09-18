/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

Sabre::Type::Erased Sabre::Type::Generic::instantiate(const std::vector<Erased> &arguments) const {
  // get the details about the incoming arguments and parameters
  auto adicity = m_parameters.size();

  // prepare the constraints cache to be used
  auto constraints = Constraints();

  // reconstruct the suitable cache now
  for (size_t ii = 0; ii < adicity; ++ii) {
    // get the incoming parameter
    auto *parameter = m_parameters.at(ii).get();

    // resolve our argument value now
    auto argument = ii < arguments.size() ? arguments.at(ii) : New::unset();

    // finally construct the necessary parameter typing
    constraints[parameter] = parameter->coalesce(argument);
  }

  // and attempt instantiating the body now
  return m_target->infer(&constraints);
}

//  PRIVATE METHODS  //

Sabre::Type::Erased Sabre::Type::Generic::m_infer(Constraints *constraints) const {
  return New::generic(m_target->infer(constraints), m_parameters);
}

bool Sabre::Type::Generic::m_unify(const Erased &candidate, Constraints *constraints) const {
  // ignore if the other is not a suitable generic
  if (!candidate->is<Generic>()) return false;

  // cast the other generic now
  auto other = candidate->as<Generic>();

  // stop if the total parameters count is invalid
  if (m_parameters.size() != other->m_parameters.size()) return false;

  // now we attempt instantiating as necessary
  auto instantiated = other->instantiate({m_parameters.cbegin(), m_parameters.cend()});

  // and finally we want to unify based on the target now
  return m_target->unify(instantiated, constraints);
}

void Sabre::Type::Generic::m_print(std::ostream &os, const Generic &self) {
  // ignore if the underlying parameters are valid here
  if (self.m_parameters.empty() || !self.m_target->is<Callable>()) return void(os << *self.m_target);

  static constexpr $::String::View s_prefix = "fn "; // prefix value
  auto suffix = fmt::to_string(*self.m_target).substr(s_prefix.size());

  // prepare the parameters to be output as well
  auto parameters = $::Ranges::To($::Ranges::Deref(self.m_parameters));

  // and show the final result now as necessary
  os << fmt::format("{0}[{1}]{2}", s_prefix, fmt::join(parameters, ", "), suffix);
}
