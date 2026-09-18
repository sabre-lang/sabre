#ifndef _SABRE_LITERAL_NUMERIC_HPP
#define _SABRE_LITERAL_NUMERIC_HPP

/// Sabre Includes
#include "sabre/forward/number.hpp"
#include "sabre/syntax/node.hpp"

namespace Sabre::Syntax {

/// @brief Number Literal Node.
class Numeric : public Mixin<Numeric, Expression> {
  //  PROPERTIES  //

  /// @brief Associated numeric value.
  Number::Floating m_value = 0;

  /// @brief Underlying view of token.
  const Lexer::Token *m_token = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a numeric from a token.
   * @param token                     Token to resolve.
   */
  explicit Numeric(const Lexer::Token *token) : m_token(token) {
    switch (token->kind()) {
    case Lexer::Kind::LTRL_NAN: m_value = NAN; break;
    case Lexer::Kind::LTRL_INF: m_value = INFINITY; break;

    case Lexer::Kind::LTRL_FLT: m_value = std::stod(buffer(), nullptr); break;
    case Lexer::Kind::LTRL_BIN: m_value = std::stoull(buffer(), nullptr, 2); break;
    case Lexer::Kind::LTRL_OCT: m_value = std::stoull(buffer(), nullptr, 8); break;
    case Lexer::Kind::LTRL_INT: m_value = std::stoull(buffer(), nullptr, 10); break;
    case Lexer::Kind::LTRL_HEX: m_value = std::stoull(buffer(), nullptr, 16); break;

    // invalid token received
    default: $_ABORT("Unexpected numeric token");
    }
  }

  /**
   * @brief Constructs a numeric node.
   * @param value                     Numeric value.
   */
  explicit Numeric(Number::Floating value = 0) : m_value(value) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the associated value.
  inline constexpr Number::Floating value() const noexcept { return m_value; }

  /// @brief Gets a buffer of the underlying numeric.
  inline constexpr $::String::Buffer buffer() const noexcept {
    return m_token ? $::String::Buffer(m_token->lexeme()) : fmt::to_string(m_value);
  }
};

} // namespace Sabre::Syntax

#endif
