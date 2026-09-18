#ifndef _SABRE_LEXER_VISITOR_HPP
#define _SABRE_LEXER_VISITOR_HPP

/// Sabre Includes
#include "sabre/lexer/buffer.hpp"

namespace Sabre::Lexer {

/// @brief Token Buffer Visitor.
class Visitor {
  //  TYPEDEFS  //

  /// @brief The underlying iterator typing.
  using Iterator = std::vector<Token>::const_iterator;

protected:
  //  PROPERTIES  //

  /// @brief A current token index.
  size_t m_index = 0;

  /// @brief Underlying token buffer.
  const std::vector<Token> &m_tokens;

  /// @brief A previous token reference.
  const Token *m_previous = &s_invalid;

  /// @brief Baseline invalid token.
  static inline const Token s_invalid = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Do not allow default construction.
  explicit Visitor() = delete;

  /**
   * @brief Constructs a token-visitor.
   * @param tokens                Buffer to encapsulate.
   */
  explicit Visitor(const Buffer *tokens) : m_tokens(tokens->buffer()) { reset(); }

  //  PUBLIC METHODS  //

  /// @brief Resets the parser-stream.
  inline constexpr void reset() { m_previous = &s_invalid, m_rewind(0); }

  /// @brief Denotes if at the EOS value.
  inline constexpr bool eos() const noexcept { return m_index >= m_tokens.size(); }

  /// @brief Gets the previous token view.
  inline constexpr const Token *previous() const { return m_previous; }

  /// @brief Gets the current token view.
  inline constexpr const Token *current() const { return eos() ? &s_invalid : m_current(); }

protected:
  //  PRIVATE METHODS  //

  /// @brief Gets the current-most token on the buffer.
  inline constexpr const Lexer::Token *m_current() const noexcept { return &m_tokens.at(m_index); }

  /**
   * @brief Handles rewinding the visitor position.
   * @param index                     Index to rewind.
   */
  inline constexpr void m_rewind(size_t index) { $_ASSERT(index <= m_tokens.size()), m_index = index; }
};

} // namespace Sabre::Lexer

#endif
