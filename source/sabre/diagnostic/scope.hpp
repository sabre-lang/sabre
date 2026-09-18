#ifndef _SABRE_DIAGNOSTIC_SCOPE_HPP
#define _SABRE_DIAGNOSTIC_SCOPE_HPP

/// Sabre Includes
#include "sabre/diagnostic/reporter.hpp"

namespace Sabre::Diagnostic {

/// @brief Encapsulates a reporter scoping.
class Scope {
  //  PROPERTIES  //

  /// @brief Enforce validation.
  bool m_valid = false;

  /// @brief Diagnostic reporter.
  Reporter *m_reporter = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Default construction.
  explicit Scope() = default;

  /**
   * @brief Constructs a reporter scoping.
   * @param reporter                  Reporter to scope.
   * @param location                  Location to enqueue.
   */
  explicit Scope(Reporter *reporter, const XLSP::Range &range) : m_valid(range != XLSP::Range()), m_reporter(reporter) {
    if (m_valid) m_reporter->m_scoping.emplace_back(m_reporter->m_resource, range);
  }

  /// @brief Handles removing the enqueued location.
  constexpr ~Scope() {
    if (m_valid && m_reporter) m_reporter->m_scoping.pop_back();
  }
};

} // namespace Sabre::Diagnostic

#endif
