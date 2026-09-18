#ifndef _SABRE_FORMAT_OPTIONS_HPP
#define _SABRE_FORMAT_OPTIONS_HPP

/// Sabre Includes
#include "sabre/forward/format.hpp"

namespace Sabre::Format {

/// @brief Formatter Options.
struct Options {
  //  PROPERTIES  //

  bool term_insert = true;     // Auto-insert terminators.
  bool newline_final = true;   // Append a trailing newline.
  bool indent_spaces = true;   // Use spaces for indentation.
  bool arrows_align = true;    // Align arrows with signatures.
  bool commas_trailing = true; // Auto-insert trailing commas.

  bool braces_expand = true;   // Forces all braces to be expanded onto newlines.
  bool braces_preserve = true; // Keep braces on same-line (KR) otherwise (Allman).

  size_t line_width = 120; // Maximum line-width.
  size_t indent_width = 4; // Indent width in spaces.

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles decoding format options.
   * @param value                     Value to decode.
   */
  static Options m_decode(const $::Serde::Value &value);
};

} // namespace Sabre::Format

#endif
