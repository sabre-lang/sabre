#ifndef _XTEST_SESSION_OPTIONS_HPP
#define _XTEST_SESSION_OPTIONS_HPP

/// Testing Includes
#include "xtest/bench/options.hpp"
#include "xtest/reporter/options.hpp"
#include "xtest/session/ordering.hpp"

namespace XT::Session {

/// @brief Core Testing Options.
struct Options {
  //  PROPERTIES  //

  /// @brief Benchmarking options.
  Bench::Options bench = {};

  /// @brief Randomization ordering.
  Ordering ordering = {};

  /// @brief Core Testing title.
  $::String::Buffer title = "xtest/anonymous";

  /// @brief Reporter options.
  Reporter::Options reporter = {};
};

} // namespace XT::Session

#endif
