#ifndef _SABRE_ANNOTATION_SIGNATURE_HPP
#define _SABRE_ANNOTATION_SIGNATURE_HPP

/// Sabre Includes
#include "sabre/syntax/annotation/constructor.hpp"

namespace Sabre::Syntax {

/// @brief Signature Annotation Node.
class Signature : public Mixin<Signature, Annotation> {
  //  PROPERTIES  //

  /// @brief Return typing.
  Annotation *m_returns;

  /// @brief Signature constructor.
  Constructor *m_prototype;

  /// @brief A defaulted parameter list available.
  static inline Parameters::List s_defaulted = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a signature annotation.
   * @param prototype                 Signature prototype.
   * @param returns                   Expected return type.
   */
  explicit Signature(Constructor *prototype, Annotation *returns) : m_returns(returns), m_prototype(prototype) {}

  //  PUBLIC METHODS  //

  inline constexpr size_t arity() const noexcept { return m_prototype->arity(); }
  inline constexpr size_t adicity() const noexcept { return m_prototype->adicity(); }
  inline constexpr const Annotation *returns() const noexcept { return m_returns; }
  inline constexpr const Variable *spread() const noexcept { return m_prototype->spread(); }
  inline constexpr const Constructor *prototype() const noexcept { return m_prototype; }
  inline constexpr const Template &generics() const noexcept { return m_prototype->generics(); }
  inline constexpr const Parameters::List &parameters() const noexcept {
    return m_prototype->parameters() ? m_prototype->parameters()->list() : s_defaulted;
  }
};

} // namespace Sabre::Syntax

#endif
