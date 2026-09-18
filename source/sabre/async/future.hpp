#ifndef _SABRE_ASYNC_FUTURE_HPP
#define _SABRE_ASYNC_FUTURE_HPP

/// Sabre Includes
#include "sabre/async/thenable.hpp"
#include "sabre/function/common.hpp"

namespace Sabre {

/// @brief Future Attributes.
template <> struct Object::Wrapper<Async::Future> {
  //  PROPERTIES  //

  /// @brief Bound arguments.
  Function::Args args = {};

  /// @brief Callback handler.
  Function::Any callback = {};

  /// @brief The bound future worker.
  Runtime::Isolate *worker = nullptr;

  /// @brief The default result value.
  $::Unique::Pointer<Async::Thenable> thenable = $::Unique::New<Async::Thenable>();

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs an anonymous future.
  explicit Wrapper() = default;

  /**
   * @brief Constructs future attributes.
   * @param callee                Calling isolate.
   * @param callback              Callback to bind.
   * @param args                  Arguments to bind.
   */
  explicit Wrapper(Runtime::Isolate *callee, const Function::Any &callback);
  explicit Wrapper(Runtime::Isolate *callee, const Function::Any &callback, Function::Args &&args);
  explicit Wrapper(Runtime::Isolate *callee, const Function::Any &callback, const Function::Args &args);

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles intializing a future.
   * @param callee                 Calling isolate.
   */
  void m_initialize(Runtime::Isolate *callee);
};

/// @brief Future Interface.
struct Async::Future : public Object::Mixin<Async::Future> {
  //  TYPEDEFS  //

  /// @brief Allow async-workers internal access.
  friend class Worker;

  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Mixin::Mixin;

  //  PUBLIC METHODS  //

  inline constexpr bool pending() const noexcept { return m_wrapper()->thenable->pending(); }
  inline constexpr bool unassigned() const noexcept { return m_wrapper()->callback.pointer().nullish(); }
  inline constexpr XSIO::Task::Status status() const noexcept { return m_wrapper()->thenable->status(); }

  inline constexpr Function::Args &args() const noexcept { return m_wrapper()->args; }
  inline constexpr Function::Any &callback() const noexcept { return m_wrapper()->callback; }
  inline constexpr Async::Thenable *thenable() const noexcept { return m_wrapper()->thenable.get(); }

  /**
   * @brief Handles waiting for a future to complete.
   * @param thread                    Virtual thread.
   */
  Result await(Thread *thread) const noexcept;
  Value::Any await(Runtime::Isolate *isolate) const noexcept;

  /**
   * @brief Waits for a future to complete within a duration.
   * @param thread                    Virtual thread.
   * @param duration                  Timeout duration.
   */
  Result timeout(Thread *thread, const $::Chrono::Duration &duration) const noexcept;
  Value::Any timeout(Runtime::Isolate *isolate, const $::Chrono::Duration &duration) const noexcept;

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Instantiates a timeout instance.
   * @param duration                  Timeout duration.
   */
  Timeout m_timeout(const $::Chrono::Duration &duration) const noexcept;

  /**
   * @brief Handlers yielding futures safely.
   * @param self                          Future instance.
   * @param yield                         Yield callback.
   */
  static void m_yield(const Future &self, Globals::Each &yield);
};

} // namespace Sabre

#endif
