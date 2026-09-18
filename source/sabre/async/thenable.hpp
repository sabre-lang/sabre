#ifndef _SABRE_ASYNC_THENABLE_HPP
#define _SABRE_ASYNC_THENABLE_HPP

/// Sabre Includes
#include "sabre/value/common.hpp"

namespace Sabre::Async {

/// @brief Asynchronous Result Typing.
using Result = std::expected<Value::Any, Value::Any>;

/// @brief Underlying Thenable Typing.
struct Thenable : public XSIO::Task::Deferred<Value::Any, Value::Any> {
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructors.
  using Deferred::Deferred;

  //  PUBLIC METHODS  //

  /// @brief Gets the underlying coalesced value.
  inline Value::Any &immediate() noexcept { return m_result.has_value() ? m_result.value() : m_result.error(); }
  inline Value::Any immediate() const noexcept { return m_result.has_value() ? *m_result : m_result.error(); }

  /**
   * @brief Handles wairint for a raw result.
   * @param thread                    Virtual thread.
   */
  Async::Result await(Thread *thread) noexcept;

  /**
   * @brief Handles waiting for a result.
   * @param isolate                   Runtime isolate.
   */
  Value::Any await(Runtime::Isolate *isolate) noexcept;

  /**
   * @brief Handles resolving a thenable.
   * @param isolate                   Runtime isolate.
   * @param value                     Value to resolve.
   */
  bool resolve(Runtime::Isolate *isolate, const Value::Any &value) noexcept;

  /**
   * @brief Handles rejecting a thenable.
   * @param isolate                   Runtime isolate.
   * @param value                     Value to reject.
   */
  bool reject(Runtime::Isolate *isolate, const Value::Any &exception) noexcept;
};

} // namespace Sabre::Async

#endif
