#ifndef _SABRE_BUNDLE_OPTIONS_HPP
#define _SABRE_BUNDLE_OPTIONS_HPP

/// Sabre Includes
#include "sabre/author/options.hpp"
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
  Author::Options author = {};

  //  CONSTRUCTORS  //

  /// @brief Constructs defaulted options.
  constexpr Options() = default;

  /**
   * @brief Constructs a set of docify options.
   * @param author            Authoring options.
   */
  constexpr Options(const Author::Options &author) : mode(Mode::DOCS), author(author) {}

  /**
   * @brief Constructs a set of bundle options.
   * @param output            Output file.
   */
  constexpr Options(Mode mode, const $::String::Buffer &output = {}) : mode(mode), author({.output = output}) {}
};

} // namespace Sabre::Bundle

#endif
