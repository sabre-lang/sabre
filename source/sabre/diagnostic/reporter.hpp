#ifndef _SABRE_DIAGNOSTIC_REPORTER_HPP
#define _SABRE_DIAGNOSTIC_REPORTER_HPP

/// Sabre Includes
#include "sabre/diagnostic/options.hpp"
#include "sabre/diagnostic/view.hpp"

namespace Sabre::Diagnostic {

/// @brief Diagnostic Reporter.
class Reporter : public XI::Transient {
  //  TYPEDEFS  //

  /// @brief Scoping internal access.
  friend class Scope;

  //  PROPERTIES  //

  /// @brief Associated reporter resource.
  $::URI::Buffer m_resource;

  /// @brief Current diagnostic scoping.
  Resource::Stack m_scoping = {};

  /// @brief Bound diagnostic values.
  std::vector<View> m_diagnostics = {};

  /// @brief Diagnostic reporter options.
  $::Unique::Pointer<Options> m_options = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a reporter instance.
   * @param options                     Reporter options.
   * @param resource                    Resource to bind.
   */
  explicit Reporter(const $::URI::Buffer &resource = {});
  explicit Reporter(const Options &options, const $::URI::Buffer &resource = {});
  explicit Reporter(XI::Container *services, const $::URI::Buffer &resource = {});

  //  PUBLIC METHODS  //

  /// @brief Clears all the available diagnostics.
  inline void clear() { m_diagnostics.clear(); }

  /// @brief Whether or not the reporter is empty.
  inline constexpr bool empty() const noexcept { return m_diagnostics.empty(); }

  /// @brief The total diagnostics count.
  inline constexpr size_t size() const noexcept { return m_diagnostics.size(); }

  /// @brief Gets the available reporter instance.
  inline constexpr $::URI::View resource() const noexcept { return m_resource; }

  /// @brief The underlying diagnostics limit.
  inline constexpr size_t limit() const noexcept { return m_options->limit; }

  /// @brief Gets the current severity assigned.
  inline constexpr Severity severity() const noexcept { return m_options->severity; }

  /// @brief Gets the total error count.
  inline constexpr size_t errors() const { return count(Severity::ERROR); }

  /// @brief Gets a view of the available diagnostics.
  inline constexpr std::vector<XLSP::Diagnostic> view() const noexcept {
    auto predicate = [](const View &diagnostic) { return diagnostic.underlying(); };
    return $::Ranges::To(m_diagnostics | std::views::transform(predicate));
  }

  /// @brief Denotes if the reporter has any error diagnostic.
  inline bool failed() const {
    auto predicate = [](const View &d) { return d.severity() == Severity::ERROR; };
    return std::ranges::any_of(m_diagnostics, predicate); // check for any errors
  }

  /**
   * @brief Counts diagnostics.
   * @param code                        Code to filter.
   */
  inline constexpr size_t count(Code code) const {
    auto predicate = [code](const View &d) { return d.code() == code; };
    return std::ranges::count_if(m_diagnostics, predicate); // count now
  }

  /**
   * @brief Counts diagnostics.
   * @param severity                    Severity to filter.
   */
  inline constexpr size_t count(Severity severity) const {
    auto predicate = [severity](const View &d) { return d.severity() == severity; };
    return std::ranges::count_if(m_diagnostics, predicate); // attempt counting now
  }

  /**
   * @brief Handles constructing diagnostics.
   * @tparam As                           Argument types.
   * @param location                      Diagnostic location.
   * @param code                          Code of diagnostic.
   * @param args                          Message arguments.
   */
  template <class... As> inline constexpr bool emit(const XLSP::Range &range, Code code, As &&...args) {
    if (range == XLSP::Range()) return emit(Resource::Stack(), code, std::forward<As>(args)...);
    return emit(Resource::Location(m_resource, range), code, std::forward<As>(args)...);
  }

  /**
   * @brief Handles constructing diagnostics.
   * @tparam As                           Argument types.
   * @param location                      Diagnostic location.
   * @param code                          Code of diagnostic.
   * @param args                          Message arguments.
   */
  template <class... As> inline constexpr bool emit(const Resource::Location &location, Code code, As &&...args) {
    auto stack = location.anonymous() ? Resource::Stack() : Resource::Stack({location});
    return emit(stack, code, std::forward<As>(args)...); // and further emit the instance now
  }

  /**
   * @brief Handles constructing diagnostics.
   * @tparam As                           Argument types.
   * @param stack                         Diagnostic locations.
   * @param code                          Code of diagnostic.
   * @param args                          Message arguments.
   */
  template <class... As> inline constexpr bool emit(const Resource::Stack &stack, Code code, As &&...args) {
    return m_emplace(m_create(code, stack), std::forward<As>(args)...);
  }

  /**
   * @brief Collects a set of diagnostics from another reporter/list.
   * @param diagnostics                   Diagnostics to collect.
   */
  inline void collect(const Reporter &reporter) { collect(reporter.m_diagnostics); }
  inline void collect(const std::vector<View> &diagnostics) {
    for (const auto &diagnostic : diagnostics) {
      if (m_ignore(diagnostic)) continue;
      if (m_exists(diagnostic)) continue;
      m_diagnostics.emplace_back(diagnostic);
    }
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Resolves a diagnostics severity (including overrides).
   * @param code                        Diagnostic code.
   */
  Severity m_severity(Code code) const;

  /**
   * @brief Checks if a diagnostic could be ignored.
   * @param view                        Diagnostic view.
   */
  bool m_ignore(const View &view) const;

  /**
   * @brief Checks if a diagnostic exists.
   * @param view                        Diagnostic view.
   */
  bool m_exists(const View &view) const;

  /**
   * @brief Constructs a diagnostic with the given details.
   * @param code                        Diagnostic code.
   * @param stack                       Resource stack.
   */
  View m_create(Code code, Resource::Stack stack) const;

  /**
   * @brief Emplaces a diagnostic onto the reporter.
   * @param view                         Diagnostic view.
   * @param args                         Message arguments.
   */
  template <class... As> inline constexpr bool m_emplace(View &&view, As &&...args) {
    // stop if we could not emplace the instance
    if (m_ignore(view)) return view.severity() == Severity::ERROR;

    // render the diagnostic now to be used (since not-ignorable)
    view.m_message = Inspect::format(view.m_code, std::forward<As>(args)...);

    // only insert if the diagnostic is unique
    if (!m_exists(view)) m_diagnostics.emplace_back(std::move(view));

    // and denote the error status now
    return view.severity() == Severity::ERROR;
  }

protected:
  //  PROTECTED METHODS  //

  /**
   * @brief Handles printing reporter diagnostics.
   * @param os                            Output stream.
   * @param self                          Reporter instance.
   */
  static void m_print(std::ostream &os, const Reporter &self);
};

} // namespace Sabre::Diagnostic

#endif
