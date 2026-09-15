#ifndef _XSIO_VIRTUAL_THREAD_HPP
#define _XSIO_VIRTUAL_THREAD_HPP

/// XSIO Includes
#include "xsio/forward/async.hpp"
#include "xsio/forward/memory.hpp"
#include "xsio/switch/context.hpp"
#include "xsio/task/executor.hpp"
#include "xsio/timer/yield.hpp"
#include "xsio/virtual/processor.hpp"
#include "xsio/virtual/worker.hpp"

namespace XSIO::Virtual {

/// @brief Virtual Thread (represents a green-thread).
class Thread {
  //  TYPEDEFS  //

  /// @brief Allow the watchdog internal access.
  friend class Async::Watchdog;

  /// @brief Allow the scheduler internal access.
  friend class Async::Scheduler;

  /// @brief Allow context switching internal access.
  friend struct Switch::Context;

  //  PROPERTIES  //

  /// @brief The current thread-state.
  $::Async::Atomic<State> m_state = State::CLEANED;

  /// @brief Last scheduled timestamp.
  $::Async::Atomic<Timer::Point> m_ts = Timer::Yield::NEVER;

  Worker *m_worker = nullptr;       // Bound virtual worker.
  Memory::Stack *m_stack = nullptr; // Acquired stack value.

  /// @brief Task executor value.
  $::Shared::Pointer<Task::Executor> m_task = nullptr;

  Switch::Pointer m_context = nullptr;   // Associated fcontext_t value.
  Switch::Callback m_callback = nullptr; // Post-context switch callback.

#if $_ASAN_ENABLED
  struct {
    size_t size = 0;
    void *stack = nullptr;
    const void *bottom = nullptr;
  } m_asan;
#endif

public:
  //  CONSTRUCTORS  //

  /// @brief Default constructor.
  constexpr Thread() = default;

  //  PUBLIC METHODS  //

  /// @brief Denotes if the thread is ready for execution.
  inline constexpr bool ready() const noexcept { return m_state == State::READY; }

  /// @brief Gets the current thread state.
  inline constexpr State state() const noexcept { return m_state.load(); }

  /// @brief Gets the attached worker for this thread.
  inline constexpr Worker *worker() const noexcept { return m_worker; }

  /// @brief Gets the bound task for this thread.
  inline constexpr Task::Executor *task() const noexcept { return m_task.get(); }

  /// @brief Gets the stack allocated for this thread.
  inline constexpr const Memory::Stack *stack() const noexcept { return m_stack; }

  /// @brief Checks if the underlying task is assigned.
  template <class... As> inline bool is() const noexcept { return m_task ? m_task->is<As...>() : false; }

  /// @brief Attempts declaring a thread as ready for execution.
  inline void awaken() { m_update(State::WAITING, State::READY); }

  /// @brief Forces a worker checkpoint to occur.
  inline void checkpoint() {
    $_ASSERT(m_worker, "Thread not assigned"), m_worker->checkpoint(); // checkpoint the worker
    if (m_ts.load() == Timer::Yield::SHOULD) Switch::Context::yield_to_scheduler(this, State::READY);
  }

  /// @brief Declares the thread as completed and reusable.
  $_NORETURN inline void exit() { m_yield(State::EXITED); }

  /**
   * @brief Declares to the manager we should fully exit.
   * @param error_code                Exit code to bind.
   */
  $_NORETURN inline void shutdown(int32_t error_code = EXIT_SUCCESS) {
    $_ASSERT(error_code > -1, "Expected a non-zero exit-code");
    auto state = static_cast<size_t>(State::SHUTDOWN) + error_code;
    $_ASSERT(state < UINT8_MAX), m_yield(static_cast<State>(state));
  }

  /**
   * @brief Handles running native callbacks.
   * @param callback                  Native callback.
   */
  inline void native(Native &&callback) {
    $_ASSERT(m_worker, "Thread not assigned");
    m_worker->native(std::move(callback));
  }

  /**
   * @brief Sleeps for the given duration.
   * @param duration                  Duration to sleep for.
   */
  inline void sleep(const Timer::Ticks &duration) { sleep(Timer::Point() + duration); }
  inline void sleep(const Timer::Point &timestamp) {
    $_ASSERT(state() == State::RUNNING); // ensure working
    m_worker->processor()->timers()->suspend(this, timestamp);
  }

  /**
   * @brief Handles assigning a thread task.
   * @param args                      Executor arguments.
   */
  template <std::derived_from<Task::Executor> T, class... As> inline T *assign(As &&...args) {
    $_ASSERT(m_task == nullptr, "Thread already assigned");
    m_update(State::CLEANED, State::WAITING); // update state
    m_task = $::Shared::New<T>(std::forward<As>(args)...);
    return m_task->m_thread = this, static_cast<T *>(m_task.get());
  }

private:
  //  PRIVATE METHODS  //

  /// @brief Handles recycling a thread instance.
  inline Memory::Stack *m_clean() {
    m_task = nullptr;
    m_worker = nullptr;
    m_context = nullptr;
    m_callback = nullptr;
    m_state = State::CLEANED;
    m_ts = Timer::Yield::NEVER;

#if $_ASAN_ENABLED
    m_asan = {};
#endif

    auto *stack = m_stack; // return
    return m_stack = nullptr, stack;
  }

  /// @brief Handles executing the underlying task.
  inline void m_execute() const noexcept { m_task->m_execute(); }

  /**
   * @brief Handles updating the current state.
   * @param expected                  Expected state.
   * @param desired                   Desired state.
   */
  inline void m_update(State expected, State desired) { m_state.compare_exchange_assert(expected, desired); }

  /**
   * @brief Handles yielding to the scheduler.
   * @param state                     State to yield.
   * @param callback                  Post-switch callback.
   */
  $_NORETURN inline void m_yield(State state, Switch::Callback &&callback = nullptr) {
    Switch::Context::yield_to_scheduler(this, state, std::move(callback)), $::System::unreachable();
  }
};

} // namespace XSIO::Virtual

#endif
