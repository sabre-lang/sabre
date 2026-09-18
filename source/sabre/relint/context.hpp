#ifndef _SABRE_RELINT_CONTEXT_HPP
#define _SABRE_RELINT_CONTEXT_HPP

/// Sabre Includes
#include "sabre/diagnostic/reporter.hpp"
#include "sabre/relint/exports.hpp"
#include "sabre/relint/hooks.hpp"
#include "sabre/relint/rule.hpp"
#include "sabre/relint/scope.hpp"
#include "sabre/relint/service.hpp"
#include "sabre/syntax/tree.hpp"

namespace Sabre::Relint {

/// @brief Limited Rule Context Handler.
class $_ABSTRACT Context {
  //  TYPEDEFS  //

  /// @brief Allow the analyzer internal access.
  friend struct Analyzer;

  //  PROPERTIES  //

  /// @brief Gets the current references scope.
  Scope *m_scope = nullptr;

  /// @brief The current target symbol.
  XLSP::Symbol *m_symbol = nullptr;

  /// @brief Services container.
  XI::Container *m_services = nullptr;

  /// @brief The currently available linting hooks/rules.
  $::Unique::Pointer<Hooks> m_hooks = nullptr;

  /// @brief Underlying diagnostics reporter.
  Diagnostic::Reporter *m_reporter = nullptr;

  /// @brief Current exports metadata value.
  $::Unique::Pointer<Exports> m_mirrors = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a defaulted context.
  explicit Context();
  explicit Context(XI::Container *services);

  virtual ~Context() = default;

  //  PUBLIC METHODS  //

  /// @brief The associate resource for the context.
  inline constexpr $::URI::View resource() const noexcept { return m_reporter->resource(); }

  /// @brief Gets the references scoping.
  inline constexpr const Scope *references() const noexcept { return m_scope; }

  /// @brief Available metadata mirrors.
  inline constexpr const Exports *mirrors() const noexcept { return m_mirrors.get(); }

  /**
   * @brief Reports a diagnostic.
   * @param location                  Resource location.
   * @param args                      Message arguments.
   */
  template <class... As> inline constexpr void report(const XLSP::Range &range, As &&...args) {
    m_report(range, std::forward<As>(args)...);
  }

  /**
   * @brief Reports a diagnostic.
   * @param node                      Syntax node.
   * @param args                      Message arguments.
   */
  template <std::derived_from<Syntax::Node> T, class... As> inline constexpr void report(const T *node, As &&...args) {
    m_report(node->trivia()->range(), std::forward<As>(args)...);
  }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Constructs a context instance.
   * @param reporter                  Reporter to bind.
   */
  inline constexpr auto m_using(Diagnostic::Reporter *reporter) {
    // construct all the hooks for our rules to be used now
    m_hooks = m_services->get<Service>()->hooks((m_reporter = reporter)->resource());

    // and allow destruction of the contextual items now
    return $::Lambda::Defer([&] { m_hooks = nullptr, m_reporter = nullptr; });
  }

  /**
   * @brief Classifies a rule into a desired diagnostic.
   * @param rule                      Rule to classify.
   */
  inline constexpr Diagnostic::Code m_classify(const Rule *rule) const noexcept {
    auto identifier = rule->documentation.identifier;
    const auto &rules = m_hooks->options()->rules;

    auto iter = rules.find(identifier); // get the potential severity iterator now
    auto severity = iter == rules.end() ? rule->severity : rules.at(identifier);

    // get the deprecated mask to be used as well
    auto deprecated = rule->deprecated ? 1 << 4 : 0;

    // prepare the necessary code now
    return 10000000 | (1 << static_cast<uint8_t>(severity)) | deprecated;
  }

  /**
   * @brief Formats linter messages.
   * @param rule                      Linter rule.
   * @param args                      Message arguments.
   */
  template <class... As> inline constexpr $::String::Buffer m_format(const Rule *rule, As &&...args) const noexcept {
    return fmt::format(fmt::runtime(rule->documentation.message), std::forward<As>(args)...);
  }

  /**
   * @brief Reports linter errors.
   * @param range                     Range to bind.
   * @param rule                      Linter rule.
   * @param args                      Message arguments.
   */
  template <class... As> inline constexpr void m_report(const XLSP::Range &range, const Rule *rule, As &&...args) {
    // get the associated code for the rule
    auto code = m_classify(rule);

    // format the message to be used
    auto message = m_format(rule, std::forward<As>(args)...);

    // and finally construct the resulting diagnostic
    m_reporter->emit(range, code, message);
  }

  /**
   * @brief Reports a diagnostic.
   * @param range                     Range to bind.
   * @param code                      Diagnostic code.
   * @param args                      Message arguments.
   */
  template <class... As> inline constexpr void m_report(const XLSP::Range &range, Diagnostic::Code code, As &&...args) {
    m_reporter->emit(range, code, std::forward<As>(args)...);
  }
};

} // namespace Sabre::Relint

#endif
