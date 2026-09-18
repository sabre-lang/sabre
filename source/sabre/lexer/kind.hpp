#ifndef _SABRE_LEXER_KIND_HPP
#define _SABRE_LEXER_KIND_HPP

/// Sabre Includes
#include "sabre/forward/lexer.hpp"

namespace Sabre::Lexer {

/// @brief Available Token Kinds.
enum class Kind : uint8_t {
#define SABRE_XX_TOKEN_BASE(N, ...) N,
#include "sabre/lexer/_defines/tokens.def"
};

} // namespace Sabre::Lexer

#endif
