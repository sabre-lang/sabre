#ifndef _SABRE_LEXER_TOKEN_HPP
#define _SABRE_LEXER_TOKEN_HPP

/// Sabre Includes
#include "sabre/lexer/inspect.hpp"

namespace Sabre::Lexer {

/// @brief Available Lexical Flags.
enum class Flag : uint16_t {
  RESERVED, // Denotes a reserved keyword.

  ASSIGNS,   // Denotes an assignment operator.
  DEFINES,   // Denotes any variable declaration.
  MODIFIER,  // Potentially a "modifier" keyword.
  ACCESSOR,  // Potentially an "accessor" keyword.
  DECORATES, // Denotes an attribute/decorator operator.
  ANNOTATES, // Denotes any valid identifier keyword.

  LEADING,    // Valid leading tokens.
  TRAILING,   // Valid trailing tokens.
  WHITESPACE, // Whitespace specific tokens.

  VOID,    // Any void literal.
  STRING,  // Any string literal.
  NUMERIC, // Any numeric literal.
  BOOLEAN, // Any boolean literal.

  SYNCABLE, // Valid synchronization token.
};

/// @brief Available Token Kinds.
class Token {
  //  PROPERTIES  //

  Kind m_kind;                     // Kind of token.
  XLSP::Range m_range = {};        // Range of token.
  $::String::Buffer m_lexeme = {}; // Raw lexeme value.

#define SABRE_XX_TOKEN_BASE(_, ...) $::Enum::Flags<Flag>(__VA_ARGS__),
  static constexpr size_t s_maximum = static_cast<size_t>(Kind::MISC_MAX) + 1;
  static constexpr std::array<$::Enum::Flags<Flag>, s_maximum> s_flags = {
#include "sabre/lexer/_defines/tokens.def"
  };

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a token value.
   * @param kind                  Token kind.
   * @param lexeme                Lexeme value.
   * @param range                 Range of token.
   */
  constexpr Token() : Token(Kind::MISC_MAX) {}
  constexpr Token(Kind kind, const XLSP::Range &range = {}) : Token(kind, Inspect::symbol(kind), range) {}
  constexpr Token(Kind kind, const $::String::View &lexeme, const XLSP::Range &range = {})
      : m_kind(kind), m_range(range), m_lexeme(lexeme) {}

  //  PUBLIC METHODS  //

  inline constexpr Kind kind() const noexcept { return m_kind; }
  inline constexpr $::String::View lexeme() const noexcept { return m_lexeme; }
  inline constexpr const XLSP::Range &range() const noexcept { return m_range; }
  inline constexpr const $::Enum::Flags<Flag> &flags() const noexcept {
    return s_flags.at(static_cast<size_t>(m_kind));
  }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles printing tokens.
   * @param os                    Output stream.
   * @param self                  Token to print.
   */
  static inline void m_print(std::ostream &os, const Token &self) {
    // resolve the incoming details to be shown
    auto position = self.m_range.start;
    auto name = Inspect::name(self.m_kind);

    // print the resulting token now for debug viewing
    os << fmt::format("[{0} / {1}] = '{2}'", name, position, self.m_lexeme);
  }
};

} // namespace Sabre::Lexer

#endif
