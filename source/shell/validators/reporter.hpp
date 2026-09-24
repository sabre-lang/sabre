#ifndef _SHELL_VALIDATORS_REPORTER_HPP
#define _SHELL_VALIDATORS_REPORTER_HPP

/// Vendor Includes
#include <CLI/CLI.hpp>
#include <xtdlib/xtdlib.hpp>

namespace Shell::Validator {

/// @brief Helper Reporter Validator.
struct Reporter : public CLI::Validator {
  //  CONSTRUCTORS  //

  /// @brief Constructs a duration validator.
  explicit Reporter() : Validator(m_check, "REPORTER") {}

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles validating reporters.
   * @param input                     Input to transform.
   */
  static $::String::Buffer m_check($::String::Buffer &input);
};

} // namespace Shell::Validator

#endif
