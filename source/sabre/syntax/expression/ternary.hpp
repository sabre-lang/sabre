#ifndef _SABRE_TERNARY_EXPRESSION_HPP
#define _SABRE_TERNARY_EXPRESSION_HPP

/// Sabre Includes
#include "sabre/syntax/node.hpp"

namespace Sabre::Syntax {

/// @brief Ternary Expression Node.
class Ternary : public Mixin<Ternary, Expression> {
  //  PROPERTIES  //

  /// @brief Conditional node.
  Expression *m_condition;

  /// @brief Assignment value.
  Expression *m_consequence;

  /// @brief Identifier name.
  Expression *m_alternative;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a ternary node.
   * @param condition                 Condition value.
   * @param consequence               Truthy result.
   * @param alternative               Falsey result.
   */
  explicit Ternary(Expression *condition, Expression *consequence, Expression *alternative)
      : m_condition(condition), m_consequence(consequence), m_alternative(alternative) {}

  //  PUBLIC METHODS  //

  inline constexpr const Expression *condition() const noexcept { return m_condition; }
  inline constexpr const Expression *consequence() const noexcept { return m_consequence; }
  inline constexpr const Expression *alternative() const noexcept { return m_alternative; }
};

} // namespace Sabre::Syntax

#endif
