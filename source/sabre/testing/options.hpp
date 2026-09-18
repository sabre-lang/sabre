#ifndef _SABRE_TESTING_OPTIONS_HPP
#define _SABRE_TESTING_OPTIONS_HPP

/// Sabre Includes
#include "sabre/forward/testing.hpp"

namespace Sabre::Testing {

/// @brief Testing Options Available.
struct Options {
  //  PROPERTIES  //

  /// @brief Denotes if randomizing testing.
  bool randomize = false;

  /// @brief Bailout total for testing.
  size_t bailout = 0;

  /// @brief Benchmarking options available.
  XT::Bench::Options bench = {};

  /// @brief Reporter options available.
  XT::Reporter::Options reporter = {};

  /// @brief The baseline pattern to use.
  $::String::Buffer pattern = "**/*.test.sabre";

  /// @brief Available filters to apply to files.
  std::vector<$::String::Buffer> filters = {};
};

} // namespace Sabre::Testing

#endif
