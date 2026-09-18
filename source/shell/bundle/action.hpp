#ifndef _SHELL_BUNDLE_ACTION_HPP
#define _SHELL_BUNDLE_ACTION_HPP

/// Sabre Includes
#include <sabre/bundle/options.hpp>

/// Shell Includes
#include "shell/command/abstract.hpp"

namespace Shell::Bundle {

/// @brief Launch Options.
using Options = Sabre::Bundle::Options;
using Runtime = Sabre::Runtime::Options;

/// @brief Bundle Command.
class Action : public Command::Abstract {
  //  PROPERTIES  //

  /// @brief Underlying bundler options.
  Options m_options = {};

  /// @brief Underlying runtime options.
  Runtime m_runtime = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a bundle action.
  explicit Action();

protected:
  //  PRIVATE METHODS  //

  /// @brief Handles executing the bundler.
  void m_execute();

  /**
   * @brief Handles subscribing the "bundle" command.
   * @param command                   CLI application.
   */
  void m_subscribe(CLI::App *command) final;
};

} // namespace Shell::Bundle

#endif
