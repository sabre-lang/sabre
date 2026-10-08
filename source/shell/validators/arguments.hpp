#ifndef _SHELL_VALIDATORS_ARGUMENTS_HPP
#define _SHELL_VALIDATORS_ARGUMENTS_HPP

/// Sabre Includes
#include <sabre/runtime/options.hpp>

namespace Shell::Validator {

/// @brief Helper Reporter Validator.
static inline constexpr void Arguments(Sabre::Runtime::Options &runtime) {
  // get some details about the arguments
  auto begin = runtime.script.argv.begin();
  auto end = runtime.script.argv.end();
  auto dashes = std::find(begin, end, "--");

  // remove any items before and including the dashes
  if (dashes != end) runtime.script.argv.erase(begin, dashes + 1);
}

} // namespace Shell::Validator

#endif
