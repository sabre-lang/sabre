#ifndef _SABRE_TOOLCHAIN_PARSE_HPP
#define _SABRE_TOOLCHAIN_PARSE_HPP

/// Sabre Includes
#include "sabre/document/buffer.hpp"
#include "sabre/forward/syntax.hpp"
#include "sabre/lexer/buffer.hpp"
#include "sabre/lexer/options.hpp"

namespace Sabre::Toolchain {

/**
 * @brief Handles lexing a token-buffer.
 * @param buffer                Source buffer view.
 * @param options               Lexer options value.
 */
Lexer::Buffer lex(const $::String::View &buffer, const Lexer::Options &options = {});
Lexer::Buffer lex(const Document::Buffer &document, const Lexer::Options &options = {});

/**
 * @brief Handles parsing a token-buffer.
 * @param tokens                Tokens to parse.
 * @param reporter              Diagnostics reporter.
 */
$::Unique::Pointer<Syntax::Tree> parse(const Lexer::Buffer *tokens, Diagnostic::Reporter *reporter = nullptr);

} // namespace Sabre::Toolchain

#endif
