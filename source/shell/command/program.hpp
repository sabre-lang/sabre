#ifndef _SHELL_COMMAND_PROGRAM_HPP
#define _SHELL_COMMAND_PROGRAM_HPP

/// Shell Includes
#include "shell/command/abstract.hpp"

namespace Shell::Command {

/// @brief The shell application.
class Program : public Usage {
  //  TYPEDEFS  //

  /// @brief Formatter Implementation.
  class Formatter : public CLI::FormatterBase {
    //  PROPERTIES  //

    /// @brief Attached program instance.
    Program *m_program;

  public:
    //  CONSTRUCTORS  //

    /**
     * @brief Constructs a formatter instance.
     * @param program               Program instance.
     */
    explicit Formatter(Program *program) : m_program(program) {}

    //  PUBLIC METHODS  //

    /**
     * @brief Handles constructing help outputs.
     * @param app                   CLI application.
     * @param name                  Name of command.
     * @param mode                  Output mode to use.
     */
    $::String::Buffer make_help(const CLI::App *app, $::String::Buffer name, CLI::AppFormatMode mode) const final;
  };

  //  PROPERTIES  //

  /// @brief Underlying application instance.
  $::Unique::Pointer<CLI::App> m_app;

  /// @brief Subscribed commands available.
  $::Map::View<$::Unique::Pointer<Abstract>> m_commands = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a program instance.
  explicit Program();

  //  PUBLIC METHODS  //

  /**
   * @brief Handles launching the program.
   * @param argc                    Argument count.
   * @param argv                    Variadic arguments.
   */
  int32_t launch(int32_t argc, char **argv);

  /**
   * @brief Handles subscribing actions.
   * @param action                  Command to subscribe.
   */
  void subscribe($::Unique::Pointer<Abstract> &&action);

  /// @brief Handles subscribing commands from the given types.
  template <std::derived_from<Abstract> T> inline void subscribe() { subscribe($::Unique::New<T>()); }

protected:
  //  PRIVATE METHODS  //

  /// @brief Handles showing the available vendors.
  static void m_vendors();

  /// @brief Handles showing the revision details.
  static void m_revision();

  /**
   * @brief Handles printing program usage.
   * @param os                        Output stream.
   */
  void m_help(std::ostream &os) const final;
};

} // namespace Shell::Command

#endif
