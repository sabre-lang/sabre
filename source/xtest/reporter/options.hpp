#ifndef _XTEST_REPORTER_OPTIONS_HPP
#define _XTEST_REPORTER_OPTIONS_HPP

/// Testing Includes
#include "xtest/forward/session.hpp"
#include "xtest/reporter/proxy.hpp"

namespace XT::Reporter {

/// @brief Factory typing for constructing reporters.
using Factory = $::Shared::Functor<$::Unique::Pointer<Stream>(Session::Runner *) const>;

/// @brief Factory typing for constructing spinners.
using Spinner = $::Shared::Functor<$::Spinner::Pointer(const $::Spinner::Options &) const>;

/**
 * @brief Ensures that a reporter exists.
 * @param label                 Reporter label.
 */
bool Ensure(const $::String::View &label);

/**
 * @brief Resolves a baseline reporter.
 * @param runner                Session runner.
 */
$::Unique::Pointer<Stream> Resolve(Session::Runner *runner);

/// @brief Reporter Options.
struct Options {
  //  PROPERTIES  //

  /// @brief The reporter output stream.
  std::ostream *output = &std::cout;

  /// @brief Defines the reporter to use.
  $::String::Buffer label = Label::DEFAULT;

  /// @brief Handles construction of reporters.
  Factory factory = Resolve;

  /// @brief Constructs a spinner instance.
  Spinner spinner = [](const $::Spinner::Options &options) { return $::Shared::New<$::Spinner::Basic>(options); };
};

} // namespace XT::Reporter

#endif
