#ifndef _SABRE_LITERAL_TEXT_HPP
#define _SABRE_LITERAL_TEXT_HPP

/// Sabre Includes
#include "sabre/syntax/node.hpp"

namespace Sabre::Syntax {

/// @brief Text Literal Node.
class Text : public Mixin<Text, Expression> {
  //  PROPERTIES  //

  /// @brief Associated text value.
  $::String::View m_view = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a tokenized text-node.
   * @param token                 Token to bind.
   */
  explicit Text(const Lexer::Token *token) : Text(token->lexeme()) {}

  /**
   * @brief Constructs an explicit text-node.
   * @param buffer                Text buffer to bind.
   */
  explicit Text(const $::String::View &view = "") : m_view(view) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the associated value view.
  inline constexpr $::String::View view() const noexcept { return m_view; }

  /// @brief Gets the associated value buffer.
  inline constexpr $::String::Buffer buffer() const noexcept { return $::String::Buffer(m_view); }
};

} // namespace Sabre::Syntax

#endif
