#ifndef _SABRE_LEXER_INSPECT_HPP
#define _SABRE_LEXER_INSPECT_HPP

/// Sabre Includes
#include "sabre/lexer/kind.hpp"

namespace Sabre::Lexer::Inspect {

/**
 * @brief Gets the associated token name.
 * @param kind                  Token kind.
 */
$::String::View name(Kind kind);

/**
 * @brief Gets the associated token symbol.
 * @param kind                  Token kind.
 */
$::String::View symbol(Kind kind);

} // namespace Sabre::Lexer::Inspect

#endif
