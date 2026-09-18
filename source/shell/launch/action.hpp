#ifndef _SHELL_LAUNCH_ACTION_HPP
#define _SHELL_LAUNCH_ACTION_HPP

/// Shell Includes
#include "shell/command/abstract.hpp"

namespace Shell::Launch {

/// @brief Launch Options.
using Options = Sabre::Runtime::Options;

/// @brief Launch Command.
class Action : public Command::Abstract {
  //  PROPERTIES  //

  /// @brief Underlying runtime options.
  Options m_runtime = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a launch action.
  explicit Action();

protected:
  //  PRIVATE METHODS  //

  /// @brief Handles executing the launcher.
  void m_execute();

  /**
   * @brief Handles subscribing the "run" command.
   * @param command                   CLI application.
   */
  void m_subscribe(CLI::App *command) final;
};

} // namespace Shell::Launch

#endif
