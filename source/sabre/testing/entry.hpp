#ifndef _SABRE_TESTING_ENTRY_HPP
#define _SABRE_TESTING_ENTRY_HPP

/// Sabre Includes
#include "sabre/async/entry.hpp"

namespace Sabre::Testing {

/// @brief Runtime Entry Thread.
class Entry : public Async::Entry {
  //  PROPERTIES  //

  /// @brief Verbose progress spinner.
  $::Spinner::Pointer m_spinner = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Async::Entry::Entry;

protected:
  //  PRIVATE METHODS  //

  /// @brief Handles launching the runtime.
  $_NORETURN void m_execute() final;

  /// @brief Declares that there are no tests available.
  void m_empty();

  /**
   * @brief Handles reporting type-errors.
   * @param reporter              Diagnostic reporter.
   * @param sources               Total testing sources.
   */
  bool m_report(Diagnostic::Reporter *reporter, size_t sources);

  /**
   * @brief Handles analyzing imports.
   * @param resource              Resource to analyze.
   * @param reporter              Diagnostic reporter.
   */
  bool m_analyze(const $::URI::Buffer &resource, Diagnostic::Reporter *reporter);

  /**
   * @brief Declares an unhandled runtime error.
   * @param resource              Resource that failed.
   * @param reason                The reason to throw.
   */
  void m_unhandled($::Map::Base<$::URI::View, Value::Any> &&errors);
  void m_unhandled(const $::URI::View &resource, const Value::Any &reason);

  /**
   * @brief Handles importing tests.
   * @param resource              Resource to import.
   * @param reporter              Global reporter instance.
   */
  Async::Thenable *m_import(const $::URI::Buffer &resource, Diagnostic::Reporter *reporter);
};

} // namespace Sabre::Testing

#endif
