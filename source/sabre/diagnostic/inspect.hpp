#ifndef _SABRE_DIAGNOSTIC_TRAITS_HPP
#define _SABRE_DIAGNOSTIC_TRAITS_HPP

/// Sabre Includes
#include "sabre/forward/diagnostic.hpp"

namespace Sabre::Diagnostic::Inspect {

/**
 * @brief Gets the associated tags.
 * @param code                  Diagnostic code.
 */
Tags tags(Code code);

/**
 * @brief Checks if a diagnostic can be overriden.
 * @param code                  Diagnostic code.
 */
bool overridable(Code code);

/**
 * @brief Gets the default severity.
 * @param code                  Diagnostic code.
 */
Severity severity(Code code);

/**
 * @brief Gets the message format.
 * @param code                  Diagnostic code.
 */
$::String::View message(Code code);

/**
 * @brief Gets the associated category.
 * @param code                  Diagnostic code.
 */
$::String::View category(Code code);

/**
 * @brief Gets the associated coloring.
 * @param severity              Severity to color.
 */
static inline constexpr $::Color::ANSI color(Severity severity) {
  switch (severity) {
  case Severity::NONE: return $::Dye::grey("NONE").bold();
  case Severity::HINT: return $::Dye::cyan("HINT").bold();
  case Severity::INFO: return $::Dye::blue("INFO").bold();
  case Severity::WARN: return $::Dye::yellow("WARN").bold();
  case Severity::ERROR: return $::Dye::red("ERROR").bold();
  default: return $::Dye::dim("UNK"); // unknown severity
  }
}

/**
 * @brief Handles formatting traits.
 * @param code                  Diagnostic code.
 * @param args                  Arguments to format.
 */
template <class... As> static inline constexpr $::String::Buffer format(Code code, As &&...args) {
  return fmt::format(fmt::runtime(message(code)), std::forward<As>(args)...);
}

} // namespace Sabre::Diagnostic::Inspect

#endif
