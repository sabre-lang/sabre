#ifndef _SABRE_BUNDLE_OPTIONS_HPP
#define _SABRE_BUNDLE_OPTIONS_HPP

/// Sabre Includes
#include "sabre/forward/bundle.hpp"

namespace Sabre::Bundle {

/// @brief Bundler Options.
struct Options {
  //  PROPERTIES  //

  /// @brief Denotes output should be compiled.
  bool compile = true;

  /// @brief Expected output file to use.
  $::String::Buffer output = "";

  //  CONSTRUCTORS  //

  /// @brief Constructs defaulted options.
  constexpr Options() = default;

  /// @brief Constructs a set of bundle options.
  constexpr Options(std::false_type) : compile(false) {}

  /**
   * @brief Constructs a set of bundle options.
   * @param output            Output file.
   */
  constexpr Options(const $::String::View &output) : output(output) {}
};

} // namespace Sabre::Bundle

#endif
