#ifndef _SHELL_SERVE_ACTION_HPP
#define _SHELL_SERVE_ACTION_HPP

/// Shell Includes
#include "shell/command/abstract.hpp"

namespace Shell::Serve {

/// @brief Server Options.
using Options = Sabre::Runtime::Options;

/// @brief Serve Command.
class Action : public Command::Abstract {
  //  PROPERTIES  //

  /// @brief Underlying runtime options.
  Options m_runtime = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a serve action.
  explicit Action();

protected:
  //  PRIVATE METHODS  //

  /// @brief Handles executing the server.
  void m_execute() const;

  /**
   * @brief Handles subscribing the "serve" command.
   * @param command                   CLI application.
   */
  void m_subscribe(CLI::App *command) final;
};

} // namespace Shell::Serve

#endif
