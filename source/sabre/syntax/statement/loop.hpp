#ifndef _SABRE_STATEMENT_LOOP_HPP
#define _SABRE_STATEMENT_LOOP_HPP

/// Syntax Includes
#include "sabre/syntax/literal/identifier.hpp"

namespace Sabre::Syntax {

/// @brief Loop Statement.
class Loop : public Mixin<Loop, Statement> {
  //  PROPERTIES  //

  /// @brief Loop statement.
  Node *m_statement;

  /// @brief Conditional expression.
  Expression *m_condition;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a loop statement.
   * @param condition                 Loop condition.
   * @param statement                 Loop statement.
   */
  explicit Loop(Expression *condition, Node *statement) : m_statement(statement), m_condition(condition) {}

  //  PUBLIC METHODS  //

  inline constexpr const Node *statement() const noexcept { return m_statement; }
  inline constexpr const Expression *condition() const noexcept { return m_condition; }
};

/// @brief For Statement.
class For : public Mixin<For, Statement> {
  //  PROPERTIES  //

  /// @brief Loop statement.
  Node *m_statement;

  /// @brief Iterable value.
  Expression *m_iterable;

  /// @brief Loop parameters available.
  std::vector<Identifier *> m_parameters;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a loop statement.
   * @param iterable                  Loop iterable.
   * @param parameters                Loop parameters.
   * @param statement                 Loop statement.
   */
  explicit For(Expression *iterable, const std::vector<Identifier *> &parameters, Node *statement)
      : m_statement(statement), m_iterable(iterable), m_parameters(parameters) {}

  //  PUBLIC METHODS  //

  inline constexpr const Node *statement() const noexcept { return m_statement; }
  inline constexpr const Expression *iterable() const noexcept { return m_iterable; }
  inline constexpr const std::vector<Identifier *> &parameters() const noexcept { return m_parameters; }
};

} // namespace Sabre::Syntax

#endif
