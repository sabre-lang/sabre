#ifndef _SABRE_PARSER_SNAPSHOT_HPP
#define _SABRE_PARSER_SNAPSHOT_HPP

/// Sabre Includes
#include "sabre/forward/parser.hpp"
#include "sabre/lexer/visitor.hpp"
#include "sabre/syntax/node.hpp"

namespace Sabre::Parser {

/// @brief Parser Bounds Snapshot.
class Snapshot {
  //  PROPERTIES  //

  /// @brief The current baseline range.
  XLSP::Range m_base = {};

  /// @brief The left-most set of bounds.
  const Lexer::Token *m_left = nullptr;

  /// @brief Current tokens cache.
  const Lexer::Visitor *m_tokens = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs an empty snapshot.
  constexpr Snapshot() = default;

  /**
   * @brief Constructs a snapshot instance.
   * @param tokens                  Tokens visitor.
   */
  constexpr Snapshot(const Lexer::Visitor *tokens) : Snapshot(tokens, tokens->current()) {}

  /**
   * @brief Constructs a snapshot instance.
   * @param tokens                  Tokens visitor.
   * @param base                    Baseline range.
   */
  constexpr Snapshot(const Lexer::Visitor *tokens, const Lexer::Token *base)
      : m_base(base->range()), m_left(base), m_tokens(tokens) {}

  //  PUBLIC METHODS  //

  /// @brief Constructs the inner set of bounds.
  inline constexpr Syntax::Bounds bounds() const noexcept {
    if ($_UNLIKELY(m_tokens == nullptr)) return Syntax::Bounds();
    else return Syntax::Bounds(m_left, m_base, m_outer());
  }

  /**
   * @brief Encloses an outside token.
   * @param base                    Token to enclose.
   */
  inline constexpr Syntax::Bounds enclose(const Lexer::Token *base) const noexcept {
    if ($_UNLIKELY(m_tokens == nullptr)) return Syntax::Bounds();
    else return Syntax::Bounds(m_left, base->range(), m_outer());
  }

  /**
   * @brief Resolves a suitable offset to use.
   * @param base                    Baseline offset.
   */
  inline constexpr Snapshot &offset(const Lexer::Token *base) noexcept { return m_base = base->range(), *this; }
  inline constexpr Snapshot &offset(const Syntax::Node *base) noexcept { return offset(base->trivia()->bounds()); }
  inline constexpr Snapshot &offset(const Syntax::Bounds &base) noexcept {
    if (base.m_base == XLSP::Range()) return *this;
    if (base.m_left < m_left) m_left = base.m_left;
    return m_base = base.outer(), *this; // update
  }

private:
  //  PRIVATE METHODS  //

  /// @brief Handles resolving the outer-most token.
  inline constexpr const Lexer::Token *m_outer() const noexcept { return m_outer(m_tokens->previous()); }
  inline constexpr const Lexer::Token *m_outer(const Lexer::Token *right) const noexcept {
    return m_left > right ? m_left : right;
  }
};

} // namespace Sabre::Parser

#endif
