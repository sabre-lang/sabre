#ifndef _SHELL_COMMAND_ABSTRACT_HPP
#define _SHELL_COMMAND_ABSTRACT_HPP

/// Vendor Includes
#include <CLI/CLI.hpp>

/// Sabre Includes
#include <sabre/runtime/options.hpp>

/// Shell Includes
#include "shell/command/usage.hpp"

namespace Shell::Command {

/// @brief Shell Command Abstraction.
class $_ABSTRACT Abstract : public Usage {
  //  TYPEDEFS  //

  /// @brief Allow shell programs internal access.
  friend class Program;

protected:
  //  PROPERTIES  //

  /// @brief The baseline descriptor value.
  Descriptor m_descriptor = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Do not allow default construction.
  explicit Abstract() = delete;

  /**
   * @brief Constructs a command instance.
   * @param name                  Name of command.
   */
  explicit Abstract(const $::String::View &name) : m_descriptor(name) {}

  /// @brief Virtual abstract destructor.
  virtual ~Abstract() = default;

  //  PUBLIC METHODS  //

  /// @brief Gets the command name.
  inline constexpr $::String::View name() const { return m_descriptor.title(); }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles subscribing a command instance.
   * @param command                   Command to be subscribed.
   */
  virtual void m_subscribe(CLI::App *command) = 0;

  /**
   * @brief Handles emplacing common options.
   * @param command                   Command to update.
   * @param options                   Runtime options.
   * @param runtime                   Flag for extra.
   */
  void m_common(CLI::App *command, Sabre::Runtime::Options *options, bool runtime);

  /**
   * @brief Handles throwing immediate failures.
   * @param message                   Failure message.
   * @param args                      Message arguments.
   */
  template <class... As> $_NORETURN inline void m_fail(fmt::format_string<As...> message, As &&...args) const {
    auto title = $::Convert::capitalize(name());
    std::cerr << $::Dye::red("Exception.{0}", title).bold() << ": ";
    std::cerr << fmt::format(message, std::forward<As>(args)...) << '\n';
    throw CLI::RuntimeError(EXIT_FAILURE); // and declare a failure
  }

  /**
   * @brief Handles showing immediate warnings.
   * @param message                   Warning message.
   * @param args                      Message arguments.
   */
  template <class... As> inline void m_warn(fmt::format_string<As...> message, As &&...args) const {
    auto title = $::Convert::capitalize(name()); // prepare the name
    std::cerr << $::Dye::yellow("Warning.{0}", title).bold() << ": ";
    std::cerr << fmt::format(message, std::forward<As>(args)...) << '\n';
  }

  /**
   * @brief Handles showing immediate warnings.
   * @param verbose                   Verbosity flag.
   * @param message                   Warning message.
   * @param args                      Message arguments.
   */
  template <class... As> inline void m_warn(bool verbose, fmt::format_string<As...> message, As &&...args) const {
    if (verbose) m_warn(message, std::forward<As>(args)...); // allowed to print
  }

  /**
   * @brief Handles emitting help information.
   * @param os                        Output stream.
   */
  inline constexpr void m_help(std::ostream &os) const final { os << m_descriptor.help(); }
};

} // namespace Shell::Command

#endif
