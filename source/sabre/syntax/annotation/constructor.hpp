#ifndef _SABRE_ANNOTATION_CONSTRUCTOR_HPP
#define _SABRE_ANNOTATION_CONSTRUCTOR_HPP

/// Sabre Includes
#include "sabre/syntax/annotation/generics.hpp"
#include "sabre/syntax/annotation/parameters.hpp"

namespace Sabre::Syntax {

/// @brief Signature Constructor Node.
class Constructor : public Mixin<Constructor, Annotation> {
  //  PROPERTIES  //

  /// @brief Generics template to be used.
  Template m_generics = {};

  /// @brief Associated signature parameters.
  Parameters *m_parameters = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a defaulted constructor.
  explicit Constructor() = default;

  /**
   * @brief Constructs a parameterized constructor.
   * @param parameters                Signature parameters.
   */
  explicit Constructor(Parameters *parameters) : m_parameters(parameters) {}

  /**
   * @brief Constructs a generic constructor.
   * @param generics                  Template generics.
   */
  explicit Constructor(const Template &generics) : m_generics(generics) {}

  /**
   * @brief Constructs a constructor.
   * @param generics                  Template generics.
   * @param parameters                Signature parameters.
   */
  explicit Constructor(const Template &generics, Parameters *parameters)
      : m_generics(generics), m_parameters(parameters) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the template generics available.
  inline constexpr const Template &generics() const noexcept { return m_generics; }

  /// @brief Gets the parameters available.
  inline constexpr const Parameters *parameters() const noexcept { return m_parameters; }

  /// @brief Gets the underlying spread parameter.
  inline constexpr const Variable *spread() const noexcept { return m_parameters ? m_parameters->spread() : nullptr; }

  /// @brief Gets the minimum parameters sizing.
  inline size_t arity() const noexcept { return m_parameters ? m_parameters->arity() : 0; }

  /// @brief Gets the maximum parameters sizing.
  inline size_t adicity() const noexcept { return m_parameters ? m_parameters->adicity() : 0; }
};

} // namespace Sabre::Syntax

#endif
