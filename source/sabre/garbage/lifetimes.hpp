#ifndef _SABRE_GARBAGE_LIFETIMES_HPP
#define _SABRE_GARBAGE_LIFETIMES_HPP

/// Sabre Includes
#include "sabre/forward/engine.hpp"
#include "sabre/forward/garbage.hpp"
#include "sabre/object/common.hpp"

/**
 * A lifetimes instance handles opening/closing disposable value stacks. As such,
 * it coordinates explicit resource management. Internally, disposables are contained
 * based on their associated `Engine::Frame`, and as such are disposed when a frame
 * goes out of scope (since they are always locally scoped). Since each disposable value
 * is "theoretically" contained outside of the lifetimes container, each value contained
 * here do not need to be manually managed (eg: via the `Globals::Roots` instance).
 */

namespace Sabre::Garbage {

/// @brief Handles Disposable Lifetimes.
class Lifetimes : public XI::Transient {
  //  TYPEDEFS  //

  /// @brief Available scoping of values.
  using Scope = std::vector<Value::Any>;

  /// @brief Available stacks to close.
  using Stack = std::vector<Scope>;

  //  PROPERTIES  //

  /// @brief Prepare the mapping.
  $::Map::Base<const Engine::Frame *, Stack> m_stacks = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs diposable lifetimes.
  explicit Lifetimes() = default;

  //  PUBLIC METHODS  //

  /**
   * @brief Attempts opening to a depth.
   * @param isolate               Runtime isolate.
   * @param depth                 Opening depth.
   */
  void open(Runtime::Isolate *isolate);
  void open(Runtime::Isolate *isolate, size_t depth);

  /**
   * @brief Closes to a required depth.
   * @param isolate               Runtime isolate.
   * @param depth                 Closing depth.
   */
  bool close(Runtime::Isolate *isolate);
  bool close(Runtime::Isolate *isolate, size_t depth);

  /**
   * @brief Value to defer for disposal.
   * @param isolate               Runtime isolate.
   * @param value                 Value to defer.
   */
  void defer(Runtime::Isolate *isolate, const Value::Any &value);

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles opening a suitable stack.
   * @param isolate               Runtime isolate.
   */
  Stack &m_open(Runtime::Isolate *isolate);

  /**
   * @brief Handles disposing a set of values.
   * @param isolate               Runtime isolate.
   * @param values                Values to dispose.
   */
  bool m_dispose(Runtime::Isolate *isolate, Scope &values);
  bool m_dispose(Runtime::Isolate *isolate, const Value::Any &value);

  /**
   * @brief Checks if an original exception should be suppressed.
   * @param isolate               Runtime isolate.
   * @param exception             Exception value.
   */
  void m_suppress(Runtime::Isolate *isolate, const Value::Any &exception);
  void m_suppress(Runtime::Isolate *isolate, const Object::Exception &exception);
};

} // namespace Sabre::Garbage

#endif
