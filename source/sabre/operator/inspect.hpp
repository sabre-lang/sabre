#ifndef _SABRE_OPERATOR_INSPECT_HPP
#define _SABRE_OPERATOR_INSPECT_HPP

/// Sabre Includes
#include "sabre/operator/kind.hpp"

namespace Sabre::Operator::Inspect {

/**
 * @brief Handles resolving an operator symbol.
 * @param kind                  Kind of operator.
 */
static inline constexpr $::String::View name(Kind kind) {
  switch (kind) {
#define X(N, S, ...)      \
  case Kind::N: return S;
#define SABRE_XX_TOKEN_UNARY(N, S, ...) X(N, S)
#define SABRE_XX_TOKEN_BINARY(N, S, ...) X(N, S)
#include "sabre/lexer/_defines/tokens.def"
    X(ASGN, "=") X(CALL, "call") SABRE_XX_OPERATORS_CUSTOM(X) default : return "unknown";
#undef X
  }
}

/**
 * @brief Handles resolving an operator kind.
 * @param symbol                Operator symbol.
 */
static inline constexpr Kind from(const $::String::View &symbol) {
  switch (XH::FNV::U32(symbol)) {
#define X(N, S, ...)                    \
  case XH::FNV::U32(S): return Kind::N;
#define SABRE_XX_TOKEN_UNARY(N, S, ...) X(N, S)
#define SABRE_XX_TOKEN_BINARY(N, S, ...) X(N, S)
#include "sabre/lexer/_defines/tokens.def"
    X(ASGN, "=")
    X(CALL, "call")
    SABRE_XX_OPERATORS_CUSTOM(X)
  default: return Kind::UNK;
#undef X
  }
}

/**
 * @brief Handles resolving attribute symbols.
 * @param kind                  Operator kind.
 */
static inline constexpr $::String::View symbol(Kind kind) {
  switch (kind) {
#define X(K, N, ...)                        \
  case Kind::K: return "#[Operator." N "]";
    X(CALL, "call")
    SABRE_XX_OPERATORS_CUSTOM(X)

  // fail for any unknown attribute operators
  default: $_ABORT("Invalid operator/attribute lookup '{0}'", name(kind));
#undef X
  }
}

} // namespace Sabre::Operator::Inspect

#endif
