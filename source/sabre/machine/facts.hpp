#ifndef _SABRE_MACHINE_FACTS_HPP
#define _SABRE_MACHINE_FACTS_HPP

/// Sabre Includes
#include "sabre/forward/machine.hpp"

namespace Sabre::Machine {

/// @brief Defines Optimization Facts.
class Facts {
  //  PROPERTIES  //

  /// @brief Denotes if there are leaked variables.
  bool m_envp = false;

  /// @brief Denotes if arguments are used.
  bool m_argv = false;

  /// @brief Denotes if parameter values are used.
  bool m_params = false;

  /// @brief Denotes if a function could panic.
  bool m_panics = false;

public:
  //  CONSTRUCTORS  //

  /// @brief Defines baseline facts.
  constexpr Facts() = default;

  /**
   * @brief Constructs a set of function facts.
   * @param callee              Information to bind.
   */
  constexpr Facts(Info *callee) { m_deduce(callee); }

  //  PUBLIC METHODS  //

  /// @brief Denotes if using the function context.
  inline constexpr bool envp() const noexcept { return m_envp; }

  /// @brief Denotes if using local arguments.
  inline constexpr bool argv() const noexcept { return m_argv; }

  /// @brief Denotes if using the parameters stack.
  inline constexpr bool params() const noexcept { return m_params; }

  /// @brief Denotes if the function could panic.
  inline constexpr bool panics() const noexcept { return m_panics; }

  /// @brief Denotes if using the frame register at all.
  inline constexpr bool any() const noexcept { return m_envp || m_argv || m_params || m_panics; }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles facts deductions.
   * @param callee              Function information.
   */
  void m_deduce(Info *callee);

  /**
   * @brief Handles reserving parameter allocations.
   * @param callee              Callee information.
   * @param instruction         Instruction to check.
   */
  void m_reserve(Info *callee, const Register::Span &span);
  void m_reserve(Info *callee, const Bytecode::Instruction &instruction);
};

} // namespace Sabre::Machine

#endif
