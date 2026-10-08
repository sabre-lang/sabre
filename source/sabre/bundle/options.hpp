#ifndef _SABRE_BUNDLE_OPTIONS_HPP
#define _SABRE_BUNDLE_OPTIONS_HPP

/// Sabre Includes
#include "sabre/forward/bundle.hpp"

namespace Sabre::Bundle {

/// @brief Available Bundling Modes.
enum class Mode : uint8_t {
  SEA,  // standalone executable application
  LINT, // only conduct the linting phase
  DOCS, // construct documentation output
};

/// @brief Bundler Options.
struct Options {
  //  PROPERTIES  //

  /// @brief Denotes output should be compiled.
  Mode mode = Mode::SEA;

  /// @brief Expected output file to use.
  $::String::Buffer output = "";

  //  CONSTRUCTORS  //

  /// @brief Constructs defaulted options.
  constexpr Options() = default;

  /**
   * @brief Constructs a set of bundle options.
   * @param output            Output file.
   */
  constexpr Options(Mode mode, const $::String::View &output = {}) : mode(mode), output(output) {}
};

} // namespace Sabre::Bundle

#endif
