#ifndef _SABRE_EXPRESSION_UNARY_HPP
#define _SABRE_EXPRESSION_UNARY_HPP

/// Sabre Includes
#include "sabre/operator/inspect.hpp"
#include "sabre/syntax/node.hpp"

namespace Sabre::Syntax {

/// @brief Unary Expression Node.
class Unary : public Mixin<Unary, Expression> {
  //  PROPERTIES  //

  /// @brief Unary operator.
  Operator::Kind m_opcode;

  /// @brief Associated expression value.
  Expression *m_operand;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a unary expression.
   * @param opcode                    Unary opcode.
   * @param operand                   Unary operand.
   */
  explicit Unary(const Lexer::Token *token, Expression *operand) : Unary(token->kind(), operand) {}
  explicit Unary(Lexer::Kind opcode, Expression *operand) : Unary(static_cast<Operator::Kind>(opcode), operand) {}
  explicit Unary(Operator::Kind opcode, Expression *operand) : m_opcode(opcode), m_operand(operand) {
    $_ASSERT(::Sabre::Operator::Unary(m_opcode), "Expected a unary opcode");
  }

  //  PUBLIC METHODS  //

  /// @brief Gets the opcode for the unary operator.
  inline constexpr Operator::Kind opcode() const noexcept { return m_opcode; }
  inline constexpr $::String::View symbol() const noexcept { return ::Sabre::Operator::Inspect::name(m_opcode); }

  /// @brief The expression operand.
  inline constexpr const Expression *operand() const noexcept { return m_operand; }
};

} // namespace Sabre::Syntax

#endif
