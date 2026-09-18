#ifndef _SABRE_LEXER_GENERATOR_IPP
#define _SABRE_LEXER_GENERATOR_IPP

/// Sabre Includes
#include "sabre/lexer/dispatch.hpp"

namespace Sabre::Lexer {

/// @brief Lexical Generator.
struct Generator {
  //  PUBLIC METHODS  //

  /// @brief Constructs a dispatch table.
  static constexpr Table table() {
    // prepare the base table to be used now
    auto dt = Table();

    // pre-define all values as invalid
    for (size_t ii = 0; ii < Dispatch::size; ++ii) dt[ii] = Dispatch::m_invalid;

#define SABRE_XX_TOKEN_SYMBOL(_, S, ...) dt[S[0]] = Dispatch::m_symbol<S[0]>;
#include "sabre/lexer/_defines/tokens.def" // define all the necessary dispatchers

    // handle all the potential numerics
    for (size_t ii = '0'; ii <= '9'; ++ii) dt[ii] = Dispatch::m_numeric;

    // and handle all potential identifier ranges
    for (size_t ii = 'a'; ii <= 'z'; ++ii) dt[ii] = Dispatch::m_identifier;
    for (size_t ii = 'A'; ii <= 'Z'; ++ii) dt[ii] = Dispatch::m_identifier;
    for (size_t ii = 0x80; ii < 0x100; ++ii) dt[ii] = Dispatch::m_identifier;

    // ensure we handle all our special cases now
    dt['"'] = Dispatch::m_string;
    dt['_'] = Dispatch::m_identifier;
    dt['\n'] = Dispatch::m_symbol<'\n'>;

    // return the resulting table now
    return dt;
  }
};

} // namespace Sabre::Lexer

#endif
