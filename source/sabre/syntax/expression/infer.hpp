#ifndef _SABRE_EXPRESSION_INFER_HPP
#define _SABRE_EXPRESSION_INFER_HPP

/// Sabre Includes
#include "sabre/syntax/node.hpp"

namespace Sabre::Syntax {

/// @brief Infer Expression Node.
class Infer : public Mixin<Infer, Expression> {
  //  PROPERTIES  //

  /// @brief Encapsulates negation of results.
  bool m_negate = false;

  /// @brief Expression value.
  Expression *m_value;

  /// @brief Expression guard.
  Annotation *m_guard;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an inferrence expression.
   * @param value                     Inferrence value.
   * @param guard                     Guard value.
   * @param negate                    Negation flag.
   */
  explicit Infer(Expression *value, Annotation *guard, bool negate = false)
      : m_negate(negate), m_value(value), m_guard(guard) {}

  //  PUBLIC METHODS  //

  inline constexpr bool negate() const noexcept { return m_negate; }
  inline constexpr const Expression *value() const noexcept { return m_value; }
  inline constexpr const Annotation *guard() const noexcept { return m_guard; }
};

} // namespace Sabre::Syntax

#endif
