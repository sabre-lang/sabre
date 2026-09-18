#ifndef _SABRE_LEXER_OPTIONS_HPP
#define _SABRE_LEXER_OPTIONS_HPP

/// Sabre Includes
#include "sabre/forward/diagnostic.hpp"
#include "sabre/forward/lexer.hpp"

namespace Sabre::Lexer {

/// @brief Lexical Encoding Type.
using Encoding = XLSP::Encoding::Type;

/// @brief Lexical Options.
struct Options {
  //  PROPERTIES  //

  /// @brief Whether comments are preserved.
  bool comments = false;

  /// @brief The baseline diagnostic encoding.
  Encoding encoding = Encoding::UTF8;

  /// @brief The diagnostic reporter to use.
  Diagnostic::Reporter *reporter = nullptr;
};

} // namespace Sabre::Lexer

#endif
