#ifndef _SHELL_TASK_BRANCH_HPP
#define _SHELL_TASK_BRANCH_HPP

/// Vendor Includes
#include <xpc/xpc.hpp>

/// Sabre Includes
#include <sabre/crate/manifest.hpp>

/// Shell Includes
#include "shell/command/abstract.hpp"

namespace Shell::Task {

/// @brief Collection of task arguments.
using Collection = std::vector<$::String::Buffer>;

/// @brief Execution Branch Details.
class Branch {
  //  TYPEDEFS  //

  /// @brief Allow the action internal access.
  friend struct Action;

  //  PROPERTIES  //

  /// @brief The current verbosity to be used.
  bool m_verbose = true;

  /// @brief Denotes script execution handling.
  bool m_parallel = false;

  /// @brief The tasks to be executed.
  Collection m_tasks = {};

  /// @brief The incoming scripts workspace.
  $::String::Buffer m_filter = ".";

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a defaulted branch value.
  constexpr Branch() = default;

  //  PUBLIC METHODS  //

  /// @brief Denotes if the branch is empty.
  inline constexpr bool empty() const noexcept { return m_tasks.empty(); }

  inline constexpr bool &verbose() noexcept { return m_verbose; }
  inline constexpr bool verbose() const noexcept { return m_verbose; }

  inline constexpr bool &parallel() noexcept { return m_parallel; }
  inline constexpr bool parallel() const noexcept { return m_parallel; }

  inline constexpr Collection &tasks() noexcept { return m_tasks; }
  inline constexpr const Collection &tasks() const noexcept { return m_tasks; }

  inline constexpr $::String::Buffer &filter() noexcept { return m_filter; }
  inline constexpr $::String::View filter() const noexcept { return m_filter; }

  /// @brief Gets the associated workspaces available.
  Collection workspaces() const noexcept;

  /**
   * @brief Handles executing a branch instance against a crate.
   * @param crate                 Crate to spawn against.
   */
  bool execute(const Sabre::Crate::Manifest *crate) const;

  /**
   * @brief Resolves tasks against a crate.
   * @param crate                 Crate to resolve against.
   */
  $::Map::Dict<$::String::Buffer> tasks(const Sabre::Crate::Manifest *crate) const;

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Prints the script to be executed.
   * @param script                Task script.
   */
  void m_prettify(const $::String::View &script) const noexcept;

  /**
   * @brief Handles executing processes sequentially or concurrently.
   * @param crate                 Crate workspace.
   */
  bool m_sequential(const Sabre::Crate::Manifest *crate) const;
  bool m_concurrent(const Sabre::Crate::Manifest *crate) const;

  /**
   * @brief Handles declaring a failure.
   * @param task                  Failure task.
   * @param errc                  Error code.
   */
  void m_failure(const $::String::View &task, int32_t errc) const;

  /**
   * @brief Handles spawning a task.
   * @param script                Task script.
   * @param crate                 Crate workspace.
   */
  XPC::Process::Child m_spawn($::String::Buffer script, const Sabre::Crate::Manifest *crate) const;

  /**
   * @brief Handles printing branches.
   * @param os                    Output stream.
   * @param self                  Branch instance.
   */
  static void m_print(std::ostream &os, const Branch &self);
};

} // namespace Shell::Task

#endif
