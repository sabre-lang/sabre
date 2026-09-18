#ifndef _SABRE_STATEMENT_MATCH_HPP
#define _SABRE_STATEMENT_MATCH_HPP

/// Syntax Includes
#include "sabre/syntax/node.hpp"

namespace Sabre::Syntax {

/// @brief Match Case Query.
class Query : public Mixin<Query> {
  //  PROPERTIES  //

  /// @brief Query statement.
  Node *m_statement;

  /// @brief Guards of the query.
  std::vector<Expression *> m_guards = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a match query.
   * @param statement           Statement node.
   * @param guards              Match guards.
   */
  explicit Query(Node *statement = nullptr, const std::vector<Expression *> &guards = {}) :
      m_statement(statement), m_guards(guards) {}

  //  PUBLIC METHODS  //

  /// @brief The query statement.
  inline constexpr const Node *statement() const noexcept { return m_statement; }

  /// @brief Guards for the query.
  inline constexpr const std::vector<Expression *> &guards() const noexcept { return m_guards; }

  /// @brief Denotes if the query contains a fallback (bypasses all conditions).
  inline constexpr bool fallback() const noexcept {
    return std::ranges::any_of(m_guards, [](const Expression *guard) { return guard->is<Fallback>(); });
  }
};

/// @brief Match Statement.
class Match : public Mixin<Match, Statement> {
  //  PROPERTIES  //

  /// @brief Value to match against.
  Expression *m_value;

  /// @brief Match queries that have been declared.
  std::vector<Query *> m_queries = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a match statement.
   * @param value                   Value to match.
   * @param queries                 Potential queries.
   */
  explicit Match(Expression *value, const std::vector<Query *> &queries = {}) : m_value(value), m_queries(queries) {}

  //  PUBLIC METHODS  //

  inline constexpr const Expression *value() const noexcept { return m_value; }
  inline constexpr const std::vector<Query *> queries() const noexcept { return m_queries; }
};

} // namespace Sabre::Syntax

#endif
