#ifndef _SABRE_SYNTAX_BOUNDS_HPP
#define _SABRE_SYNTAX_BOUNDS_HPP

/// Sabre Includes
#include "sabre/forward/parser.hpp"
#include "sabre/forward/relint.hpp"
#include "sabre/forward/syntax.hpp"
#include "sabre/lexer/token.hpp"

namespace Sabre::Syntax {

/// @brief Encapsulates Node Bounds.
class Bounds {
  //  TYPEDEFS  //

  /// @brief Allow the parser stream snapshot access.
  friend class Parser::Snapshot;

  /// @brief Allow linter comments internal access.
  friend class Relint::Comments;

  //  PROPERTIES  //

  /// @brief The baseline range to enclose.
  XLSP::Range m_base = {};

  /// @brief Left-most token.
  const Lexer::Token *m_left = nullptr;

  /// @brief Right-most token.
  const Lexer::Token *m_right = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs an anonymous location.
  explicit constexpr Bounds() = default;

  /**
   * @brief Constructs a simple set of bounds.
   * @param base                    Baseline token.
   */
  explicit constexpr Bounds(const Lexer::Token *base) : m_base(base->range()), m_left(base), m_right(base) {}

  /**
   * @brief Constructs a complex set of bounds.
   * @param left                    Left-most token.
   * @param right                   Right-most token.
   */
  explicit constexpr Bounds(const Lexer::Token *left, const Lexer::Token *right) :
      m_base(left->range(), right->range()), m_left(left), m_right(right) {}

  /**
   * @brief Constructs a complex set of bounds.
   * @param left                    Left-most token.
   * @param base                    Baseline token.
   * @param right                   Right-most token.
   */
  explicit constexpr Bounds(const Lexer::Token *left, const Lexer::Token *base, const Lexer::Token *right) :
      Bounds(left, base->range(), right) {}

  /**
   * @brief Constructs a complex set of bounds.
   * @param left                    Left-most token.
   * @param base                    Baseline range.
   * @param right                   Right-most token.
   */
  explicit constexpr Bounds(const Lexer::Token *left, const XLSP::Range &base, const Lexer::Token *right) :
      m_base(base), m_left(left), m_right(right) {}

  //  OPERATOR METHODS  //

  /// @brief Allow casting the bounds to the outer-most range.
  inline constexpr operator XLSP::Range() const noexcept { return outer(); }

  //  PUBLIC METHODS  //

  /// @brief Gets the inner-most range.
  inline constexpr XLSP::Range inner() const noexcept { return m_base; }

  /// @brief Gets the outer-most range.
  inline constexpr XLSP::Range outer() const noexcept {
    if (m_left == nullptr || m_right == nullptr) return {};
    return {m_left->range().start, m_right->range().end};
  }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Debug printing for bounds.
   * @param os                        Output stream.
   * @param self                      Bounds instance.
   */
  static inline void m_print(std::ostream &os, const Bounds &self) {
    os << "I: " << self.inner() << " / O: " << self.outer();
  }
};

} // namespace Sabre::Syntax

#endif
