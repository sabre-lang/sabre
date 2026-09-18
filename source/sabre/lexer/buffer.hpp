#ifndef _SABRE_LEXER_BUFFER_HPP
#define _SABRE_LEXER_BUFFER_HPP

/// Sabre Includes
#include "sabre/lexer/token.hpp"

namespace Sabre::Lexer {

/// @brief Token Buffer Container.
class Buffer {
  //  PROPERTIES  //

  /// @brief Denotes if comments are preserved.
  bool m_comments = false;

  /// @brief Attached buffer of tokens for this container.
  std::vector<Token> m_tokens = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a token buffer.
   * @param comments              Comments flag.
   */
  constexpr Buffer(bool comments = false) : m_comments(comments) {}

  //  PUBLIC METHODS  //

  /// @brief Denotes if there are comments preserved in the buffer.
  inline constexpr bool comments() const noexcept { return m_comments; }

  /// @brief Denotes if the tokens are empty.
  inline constexpr bool empty() const noexcept { return m_tokens.empty(); }

  /// @brief Gets the total tokens bound.
  inline constexpr size_t size() const noexcept { return m_tokens.size(); }

  /// @brief Gets the underlying token buffer.
  inline constexpr std::vector<Token> &buffer() { return m_tokens; }
  inline constexpr const std::vector<Token> &buffer() const { return m_tokens; }

  /**
   * @brief Handles appending a token.
   * @param token                 Token to append.
   */
  inline constexpr void append(Token &&token) {
    $_ASSERT(token.kind() != Kind::MISC_MAX, "Invalid token");
    if (token.kind() == Kind::MISC_CMT && !m_comments) return;
    m_tokens.emplace_back(std::move(token)); // valid token
  }
};

} // namespace Sabre::Lexer

#endif
