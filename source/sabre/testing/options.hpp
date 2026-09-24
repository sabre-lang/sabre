#ifndef _SABRE_TESTING_OPTIONS_HPP
#define _SABRE_TESTING_OPTIONS_HPP

/// Sabre Includes
#include "sabre/forward/testing.hpp"

namespace Sabre::Testing {

/// @brief Testing Options Available.
struct Options {
  //  PROPERTIES  //

  /// @brief Bailout total for testing.
  size_t bailout = 0;

  /// @brief Benchmarking options available.
  XT::Bench::Options bench = {};

  /// @brief Randomization ordering.
  XT::Session::Ordering ordering = {};

  /// @brief Reporter output file value.
  $::String::Buffer outfile = {};

  /// @brief The baseline file pattern to use.
  $::String::Buffer pattern = "**/*.test.sabre";

  /// @brief Reporter options available.
  $::String::Buffer reporter = XT::Reporter::Label::DEFAULT;

  /// @brief Available filters to apply to testing names.
  std::vector<$::String::Buffer> filters = {};
};

} // namespace Sabre::Testing

#endif
