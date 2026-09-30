#ifndef _XSIO_ASYNC_MANAGER_HPP
#define _XSIO_ASYNC_MANAGER_HPP

/// XSIO Includes
#include "xsio/async/options.hpp"
#include "xsio/async/scheduler.hpp"
#include "xsio/async/shutdown.hpp"
#include "xsio/async/storage.hpp"
#include "xsio/async/watchdog.hpp"
#include "xsio/task/spinner.hpp"
#include "xsio/task/timeout.hpp"

namespace XSIO::Async {

/// @brief Asynchronous Runtime Manager.
class Manager {
  //  PROPERTIES  //

  /// @brief Global manager mutex.
  mutable $::Mutex::Auto m_mutex;

  /// @brief Available runtime storage.
  $::Unique::Pointer<Storage> m_storage = nullptr;

  /// @brief Associated watchdog instance.
  $::Unique::Pointer<Watchdog> m_watchdog = nullptr;

  /// @brief Associated scheduler instance.
  $::Unique::Pointer<Scheduler> m_scheduler = nullptr;

  /// @brief Internal shutdown handler.
  $::Unique::Pointer<Shutdown> m_shutdown = nullptr;

  /// @brief Associated signal handler.
  $::Unique::Pointer<$::Signal::Handler> m_signals = nullptr;

  /// @brief Readiness signaler.
  $::Async::Signaller m_ready = m_mutex;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a manager instance.
   * @param options                   Manager options.
   */
  explicit Manager();
  explicit Manager(const Options *options);

  //  PUBLIC METHODS  //

  /// @brief Denotes if the runtime is still running.
  inline constexpr bool running() const noexcept { return !m_shutdown->state(); }

  /// @brief Gets the runtime options.
  inline constexpr const Options *options() const noexcept { return m_storage->options; }

  /// @brief Gets the bound scheduler.
  inline constexpr Scheduler *scheduler() const noexcept { return m_scheduler.get(); }

  /// @brief Gets the available runtime storage.
  inline constexpr Storage *storage() const noexcept { return m_storage.get(); }

  /// @brief Gets the internal signal handler.
  inline constexpr $::Signal::Handler *signals() const noexcept { return m_signals.get(); }

  /// @brief Gets the shutdown handler.
  inline constexpr Shutdown *shutdown() const noexcept { return m_shutdown.get(); }

  /// @brief Blocks until the manager is ready.
  inline constexpr void ready() { m_ready.wait(); }

  /// @brief Handles launching the manager.
  inline constexpr int32_t launch() {
    // wait for exit to occur
    m_shutdown->wait();

    // join all our features
    m_scheduler->join();
    m_watchdog->join();

    // allow cleanup code to occur here
    m_storage->options->cleanup();

    // and return the necessary error-code
    return m_shutdown->code();
  }

  /**
   * @brief Handles spawning tasks.
   * @param args                      Arguments to forward.
   */
  template <std::derived_from<Task::Executor> T, class... As> inline constexpr T *spawn(As &&...args) {
    return m_scheduler->schedule<T>(std::forward<As>(args)...);
  }

  /**
   * @brief Handles launching the manager with an entry executor.
   * @param args                      Arguments to forward.
   */
  template <std::derived_from<Task::Executor> T, class... As> inline constexpr int32_t launch(As &&...args) {
    return spawn<T>(std::forward<As>(args)...), launch();
  }

  /**
   * @brief Constructs a spinner.
   * @param args                      Arguments to forward.
   */
  template <class... As> inline $::Spinner::Pointer spinner(As &&...args) {
    auto spinner = $::Shared::New<Task::Spinner>(std::forward<As>(args)...);
    return spinner->m_schedule(m_scheduler.get()), spinner; // schedule now
  }

  /**
   * @brief Constructs a timeout.
   * @param args                      Arguments to forward.
   */
  template <class... As> inline $::Unique::Pointer<Task::Timeout> timeout(As &&...args) {
    return $::Unique::New<Task::Timeout>(m_scheduler.get(), std::forward<As>(args)...);
  }

  /**
   * @brief Handles exiting the runtime.
   * @param exit_code                 Exit-code to return.
   */
  inline constexpr void exit(int32_t exit_code = EXIT_SUCCESS) { m_shutdown->request(exit_code); }
};

} // namespace XSIO::Async

#endif
