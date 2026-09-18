#ifndef _SABRE_DIAGNOSTIC_VIEW_HPP
#define _SABRE_DIAGNOSTIC_VIEW_HPP

/// Sabre Includes
#include "sabre/diagnostic/inspect.hpp"
#include "sabre/resource/location.hpp"

namespace Sabre::Diagnostic {

/// @brief Compact Diagnostic View.
class View {
  //  TYPEDEFS  //

  /// @brief Allow the reporter internal access.
  friend class Reporter;

  //  PROPERTIES  //

  /// @brief Diagnostic code.
  Code m_code = -1;

  /// @brief Stacktrace for a diagnostic.
  Resource::Stack m_stack = {};

  /// @brief Bound message value.
  $::String::Buffer m_message = "...";

  /// @brief Bound severity value.
  Severity m_severity = Severity::ERROR;

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a defaulted diagnostic.
  constexpr View() = default;

  /**
   * Constructs a view from a code.
   * @param code                    Diagnostic code.
   * @param stack                   Resource stack.
   */
  constexpr View(Code code, const Resource::Stack &stack = {}) : View(code, Inspect::severity(code), stack) {}

  /**
   * Constructs a view from the given options.
   * @param code                    Diagnostic code.
   * @param severity                Override severity.
   * @param stack                   Resource stack.
   */
  constexpr View(Code code, Severity severity, const Resource::Stack &stack = {}) :
      m_code(code), m_stack(stack), m_severity(severity) {}

  //  OPERATOR METHODS  //

  /// @brief Checks if diagnostics are equal.
  inline constexpr auto operator==(const View &other) const {
    if (std::tie(m_code, m_message) != std::tie(other.m_code, other.m_message)) return false;
    return (m_stack.empty() && other.m_stack.empty()) || m_stack.front() == other.m_stack.front();
  }

  //  PUBLIC METHODS  //

  /// @brief Gets the diagnostic code.
  inline constexpr Code &code() noexcept { return m_code; }
  inline constexpr Code code() const noexcept { return m_code; }

  /// @brief Gets the diagnostic severity.
  inline constexpr Severity &severity() noexcept { return m_severity; }
  inline constexpr Severity severity() const noexcept { return m_severity; }

  /// @brief Gets the diagnostic message.
  inline constexpr $::String::Buffer &message() noexcept { return m_message; }
  inline constexpr $::String::View message() const noexcept { return m_message; }

  /// @brief Gets the associated resource stack.
  inline constexpr Resource::Stack &stack() noexcept { return m_stack; }
  inline constexpr const Resource::Stack &stack() const noexcept { return m_stack; }

  /// @brief Resolves diagnostic tags from the current code.
  inline constexpr Tags tags() const noexcept { return Inspect::tags(m_code); }

  /// @brief Resolves categories from the current code.
  inline constexpr $::String::View category() const noexcept { return Inspect::category(m_code); }

  /// @brief Converts a diagnostic to its underlying LSP equivalent.
  inline constexpr XLSP::Diagnostic underlying() const noexcept {
    // prepare the baseline underlying diagnostic to be constructed
    auto diagnostic = XLSP::Diagnostic();

    // update each item as necessary
    diagnostic.code = m_code;
    diagnostic.source = "TAL";
    diagnostic.message = m_message;
    diagnostic.severity = m_severity;

    // tags should be resolved dynamically
    diagnostic.tags = tags();

    // update all the conditional items required
    if (m_stack.size()) diagnostic.range = m_stack.back().range().client();

    // and return the resulting diagnostic
    return diagnostic;
  }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles printing a diagnostic.
   * @param os                        Output stream.
   * @param self                      Diagnostic view.
   */
  static void m_print(std::ostream &os, const View &self);
};

} // namespace Sabre::Diagnostic

#endif
