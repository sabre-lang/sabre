#ifndef _SHELL_DOCIFY_ACTION_HPP
#define _SHELL_DOCIFY_ACTION_HPP

/// Sabre Includes
#include <sabre/bundle/options.hpp>

/// Shell Includes
#include "shell/command/abstract.hpp"

namespace Shell::Docify {

/// @brief Docification Options.
using Options = Sabre::Runtime::Options;

/// @brief Docification Command.
class Action : public Command::Abstract {
  //  PROPERTIES  //

  /// @brief Output file to write documentation.
  $::String::Buffer m_output = {};

  /// @brief Underlying runtime options.
  Options m_runtime = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a docify action.
  explicit Action();

protected:
  //  PRIVATE METHODS  //

  /// @brief Handles executing docification.
  void m_execute();

  /**
   * @brief Handles subscribing the "docify" command.
   * @param command                   CLI application.
   */
  void m_subscribe(CLI::App *command) final;
};

} // namespace Shell::Docify

#endif
