#ifndef _SABRE_STATEMENT_CONDITIONAL_HPP
#define _SABRE_STATEMENT_CONDITIONAL_HPP

/// Sabre Includes
#include "sabre/syntax/node.hpp"

namespace Sabre::Syntax {

/// @brief Conditional Statement.
class Conditional : public Mixin<Conditional, Statement> {
  //  PROPERTIES  //

  /// @brief Truthy consequence.
  Node *m_consequence;

  /// @brief Falsey alternative.
  Node *m_alternative;

  /// @brief Conditional expression.
  Expression *m_condition;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a block statement.
   * @param condition                 Base condition.
   * @param consequence               Truthy block.
   * @param alternative               Falsey block.
   */
  explicit Conditional(Expression *condition, Node *consequence, Node *alternative = nullptr)
      : m_consequence(consequence), m_alternative(alternative), m_condition(condition) {}

  //  PUBLIC METHODS  //

  inline constexpr const Node *consequence() const noexcept { return m_consequence; }
  inline constexpr const Node *alternative() const noexcept { return m_alternative; }
  inline constexpr const Expression *condition() const noexcept { return m_condition; }
};

} // namespace Sabre::Syntax

#endif
