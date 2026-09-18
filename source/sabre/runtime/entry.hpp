#ifndef _SABRE_RUNTIME_ENTRY_HPP
#define _SABRE_RUNTIME_ENTRY_HPP

/// Sabre Includes
#include "sabre/async/entry.hpp"

namespace Sabre::Runtime {

/// @brief Runtime Entry Thread.
struct Entry : public Async::Entry {
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Async::Entry::Entry;

protected:
  //  PRIVATE METHODS  //

  /// @brief Handles launching the runtime.
  $_NORETURN void m_execute() final;

  /**
   * @brief Handles importing a script.
   * @param script                Script to import.
   */
  Value::Any m_import(const $::String::View &script);

  /**
   * @brief Forces type-checking to occur.
   * @param resource              Base resource.
   */
  Value::Any m_analyze(const $::URI::Buffer &resource);
};

} // namespace Sabre::Runtime

#endif
