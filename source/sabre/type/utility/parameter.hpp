#ifndef _SABRE_TYPE_PARAMETER_HPP
#define _SABRE_TYPE_PARAMETER_HPP

/// Sabre Includes
#include "sabre/type/registry.hpp"

namespace Sabre::Type {

/// @brief Parameter Typing.
class Parameter : public Mixin<Parameter> {
  //  PROPERTIES  //

  /// @brief Parameter name.
  $::String::View m_name;

  /// @brief Extension type.
  Erased m_extends = New::any();

  /// @brief Fallback type.
  Erased m_fallback = New::unset();

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a type-parameter.
   * @param name                  Name of parameter.
   * @param extends               Extension type.
   * @param fallback              Fallback type.
   */
  explicit Parameter(const $::String::View &name) : m_name(name) {}
  explicit Parameter(const $::String::View &name, const Erased &extends) : m_name(name), m_extends(extends) {}
  explicit Parameter(const $::String::View &name, const Erased &extends, const Erased &fallback) :
      m_name(name), m_extends(extends), m_fallback(fallback) {}

  //  PROPERTIES  //

  /// @brief Gets the name of the parameter.
  inline constexpr $::String::View name() const noexcept { return m_name; }

  /// @brief Gets the extension typing for the parameter.
  inline constexpr const Erased &extends() const noexcept { return m_extends; }

  /// @brief Gets the fallback typing for the parameter.
  inline constexpr const Erased &fallback() const noexcept { return m_fallback; }

  /// @brief Denotes if a type is required.
  inline constexpr bool required() const noexcept { return m_fallback->is<Unset>(); }

  /// @brief Gets the baseline truthiness of the parameter.
  inline constexpr $::Unit::Ternary truthiness() const noexcept final { return m_extends->truthiness(); }

  /// @brief Allows cloning instances of parameters.
  inline constexpr Erased clone() const noexcept { return $::Shared::New<Parameter>(*this); }

  /// @brief Handles coalescing parameter types.
  inline constexpr const Erased &coalesce() const noexcept { return required() ? m_extends : m_fallback; }
  inline constexpr const Erased &coalesce(const Erased &target) const noexcept {
    return target->is<Unset>() ? coalesce() : target;
  }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles instantiating parameters.
   * @param constraints               Generic constraints.
   */
  Erased m_infer(Constraints *constraints) const final;

  /**
   * @brief Handles running a unification pass.
   * @param candidate                 Candidate to unify.
   * @param constraints               Generic constraints.
   */
  bool m_unify(const Erased &candidate, Constraints *constraints) const final;

  /**
   * @brief Handles printing the type.
   * @param os                        Output stream.
   * @param self                      Parameter type.
   */
  static inline void m_print(std::ostream &os, const Parameter &self) {
    os << self.m_name; // show the base name before details
    if (!self.m_extends->is<Any>()) os << ": " << *self.m_extends;
    if (!self.m_fallback->is<Unset>()) os << " = " << *self.m_fallback;
  }
};

} // namespace Sabre::Type

#endif
