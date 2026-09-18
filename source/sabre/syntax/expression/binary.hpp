#ifndef _SABRE_EXPRESSION_BINARY_HPP
#define _SABRE_EXPRESSION_BINARY_HPP

/// Sabre Includes
#include "sabre/operator/inspect.hpp"
#include "sabre/syntax/node.hpp"

namespace Sabre::Syntax {

/// @brief Binary Expression Node.
class Binary : public Mixin<Binary, Expression> {
  //  PROPERTIES  //

  /// @brief Binary operator.
  Operator::Kind m_opcode;

  Expression *m_left;  // LHS expression.
  Expression *m_right; // RHS expression.

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a binary expression.
   * @param token                     Binary token.
   * @param left                      LHS expression.
   * @param right                     RHS expression.
   */
  explicit Binary(const Lexer::Token *token, Expression *left, Expression *right)
      : Binary(token->kind(), left, right) {}

  /**
   * @brief Constructs a binary expression.
   * @param opcode                    Binary opcode.
   * @param left                      LHS expression.
   * @param right                     RHS expression.
   */
  explicit Binary(Lexer::Kind opcode, Expression *left, Expression *right)
      : Binary(static_cast<Operator::Kind>(opcode), left, right) {}

  /**
   * @brief Constructs a binary expression.
   * @param opcode                    Binary opcode.
   * @param left                      LHS expression.
   * @param right                     RHS expression.
   */
  explicit Binary(Operator::Kind opcode, Expression *left, Expression *right)
      : m_opcode(opcode), m_left(left), m_right(right) {
    $_ASSERT(::Sabre::Operator::Binary(m_opcode), "Expected a binary opcode");
  }

  //  PUBLIC METHODS  //

  inline constexpr const Expression *left() const noexcept { return m_left; }
  inline constexpr const Expression *right() const noexcept { return m_right; }
  inline constexpr Operator::Kind opcode() const noexcept { return m_opcode; }
  inline constexpr $::String::View symbol() const noexcept { return ::Sabre::Operator::Inspect::name(m_opcode); }
};

} // namespace Sabre::Syntax

#endif
