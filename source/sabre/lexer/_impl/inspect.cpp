/// Sabre Includes
#include "sabre/lexer/inspect.hpp"

$::String::View Sabre::Lexer::Inspect::name(Kind kind) {
  switch (kind) {
#define SABRE_XX_TOKEN_BASE(N, ...) \
  case Kind::N: return #N;
#include "sabre/lexer/_defines/tokens.def"
  default: return "MISC_UNK";
  }
}

/**
 * @brief Gets the associated token symbol.
 * @param kind                  Token kind.
 */
$::String::View Sabre::Lexer::Inspect::symbol(Kind kind) {
  switch (kind) {
#define SABRE_XX_TOKEN_KEYWORD(N, S, ...) SABRE_XX_TOKEN_SYMBOL(N, S)
#define SABRE_XX_TOKEN_SYMBOL(N, S, ...) \
  case Kind::N: return S;
#include "sabre/lexer/_defines/tokens.def"
  default: return "";
  }
}
