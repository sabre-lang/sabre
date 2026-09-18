#ifndef _SABRE_LITERAL_IDENTIFIER_HPP
#define _SABRE_LITERAL_IDENTIFIER_HPP

/// Sabre Includes
#include "sabre/syntax/node.hpp"

namespace Sabre::Syntax {

/// @brief Self Literal Node.
struct Self : public Mixin<Self, Expression> {
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  explicit Self() = default;

  //  PUBLIC METHODS  //

  /// @brief Associated name of the identifier.
  inline constexpr $::String::View name() const noexcept { return "Self"; }
};

/// @brief Identifier Literal Node.
class Identifier : public Mixin<Identifier, Expression> {
  //  PROPERTIES  //

  /// @brief Associated identifier value.
  $::String::View m_name;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a named identifier.
   * @param name                  Name to bind.
   */
  explicit Identifier(const $::String::View &name) : m_name(name) {}

  /**
   * @brief Constructs a tokenized identifier.
   * @param token                 Token to bind.
   */
  explicit Identifier(const Lexer::Token *token) : Identifier(token->lexeme()) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the associated value.
  inline constexpr $::String::View name() const noexcept { return m_name; }
};

} // namespace Sabre::Syntax

#endif
