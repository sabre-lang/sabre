#ifndef _SHELL_FORMAT_ACTION_HPP
#define _SHELL_FORMAT_ACTION_HPP

/// Sabre Includes
#include <sabre/format/options.hpp>

/// Shell Includes
#include "shell/command/abstract.hpp"

namespace Shell::Format {

/// @brief Format Result Typing.
using Result = Sabre::Format::Result;

/// @brief Available Format Options.
using Options = Sabre::Format::Options;

/// @brief Format Command.
class Action : public Command::Abstract {
  //  PROPERTIES  //

  bool m_json = false;  // Output as JSON.
  bool m_write = false; // Overwrite values.

  /// @brief Baseline formatter options.
  Options m_options = {};

  /// @brief Available formatting targets.
  std::vector<$::String::View> m_targets = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a format action.
  explicit Action();

protected:
  //  PRIVATE METHODS  //

  /// @brief Handles executing the formatter.
  void m_execute();

  /**
   * @brief Handles subscribing the "format" command.
   * @param command                   CLI application.
   */
  void m_subscribe(CLI::App *command) final;

  /**
   * @brief Handles formatting a file instance.
   * @param resource                  Script resource.
   */
  Result m_worker(const $::URI::Buffer &resource) const;

  /**
   * @brief Overwrites file contents on success.
   * @param script                    Script to output.
   * @param result                    Result value.
   */
  void m_overwrite(const $::URI::View &resource, const Result &result) const;

  /**
   * @brief Outputs results to the console.
   * @param script                    Script to output.
   * @param result                    Result value.
   * @param edits                     JSON edits flag.
   */
  void m_output(const $::URI::View &resource, const Result &result, bool edits) const;
};

} // namespace Shell::Format

#endif
