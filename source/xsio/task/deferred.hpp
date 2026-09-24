#ifndef _XSIO_TASK_DEFERRED_HPP
#define _XSIO_TASK_DEFERRED_HPP

/// XSIO Includes
#include "xsio/switch/context.hpp"
#include "xsio/virtual/thread.hpp"

namespace XSIO::Task {

/// @brief Potential Task Status.
enum class Status : uint8_t { PENDING, REJECTED, RESOLVED };

/// @brief Thenable Deferred Structure.
template <class R, class E = std::future_error> class Deferred {
  //  TYPEDEFS  //

  /// @brief Expected result value.
  using Result = std::expected<R, E>;

  /// @brief Internal queue representation.
  struct Queue {
    size_t used;               // Used queue size.
    size_t capacity;           // Current capacity.
    Virtual::Thread *buffer[]; // Buffer of threads.
  };

  /// @brief Ensure the instance is default constructible.
  static_assert(std::is_default_constructible_v<R>);

protected:
  //  PROPERTIES  //

  /// @brief Bound mutex value.
  mutable $::Mutex::Light m_mutex;

  /// @brief Bound deferred queue.
  Queue *m_queue = nullptr;

  /// @brief The result for a deferred.
  Result m_result = Result();

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs an empty deferred value.
  constexpr Deferred() : m_queue(m_allocate()) {}

  /**
   * @brief Constructs an immediately fulfilled result.
   * @param value             Value to bind.
   */
  template <class U>
    requires std::disjunction_v<std::is_same<U, R>, std::is_same<U, E>>
  constexpr Deferred(const U &value) : m_result(value) {}

  /**
   * @brief Constructs an immediately fulfilled result.
   * @param value             Value to bind.
   */
  template <class U>
    requires std::disjunction_v<std::is_same<U, R>, std::is_same<U, E>>
  constexpr Deferred(U &&value) : m_result(std::move(value)) {}

  /// @brief Handles safely removing deferreds.
  constexpr ~Deferred() {
    $_UNUSED $_AUTO = $::Lock::guard(m_mutex);
    m_queue = m_release(m_queue); // and release
  }

  //  PUBLIC METHODS  //

  /// @brief Denotes if the instance is still pending.
  inline constexpr bool pending() const noexcept { return m_queue != nullptr; }

  /// @brief Gets a suitably named status value.
  inline constexpr Status status() const noexcept {
    if (pending()) return Status::PENDING; // typically will be here
    return m_result.has_value() ? Status::RESOLVED : Status::REJECTED;
  }

  /**
   * @brief Handles waiting for the result.
   * @param thread                Virtual thread.
   */
  inline Result await(Virtual::Thread *thread) noexcept { return m_await(thread), m_result; }

  /**
   * @brief Handles resolving a deferred.
   * @param thread                Virtual thread.
   * @param value                 Value to set.
   */
  inline bool resolve(Virtual::Thread *thread, const R &value) {
    $_UNUSED $_AUTO = $::Lock::guard(m_mutex);
    if (!pending()) return false; // not pending
    return m_result = value, m_notify(thread), true;
  }

  /**
   * @brief Handles resolving a deferred.
   * @param thread                Virtual thread.
   * @param value                 Value to set.
   */
  inline bool resolve(Virtual::Thread *thread, R &&value) {
    $_UNUSED $_AUTO = $::Lock::guard(m_mutex);
    if (!pending()) return false; // not pending
    return m_result = std::move(value), m_notify(thread), true;
  }

  /**
   * @brief Handles rejecting a deferred.
   * @param thread                Virtual thread.
   * @param exception             Value to set.
   */
  inline bool reject(Virtual::Thread *thread, const E &exception) {
    $_UNUSED $_AUTO = $::Lock::guard(m_mutex);
    if (!pending()) return false; // not pending
    return m_result = std::unexpected(exception), m_notify(thread), true;
  }

  /**
   * @brief Handles rejecting a deferred.
   * @param thread                Virtual thread.
   * @param value                 Value to set.
   */
  inline bool reject(Virtual::Thread *thread, E &&exception) {
    $_UNUSED $_AUTO = $::Lock::guard(m_mutex);
    if (!pending()) return false; // not pending
    return m_result = std::unexpected(std::move(exception)), m_notify(thread), true;
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Waits for the result to change.
   * @param thread                Virtual thread.
   */
  inline void m_await(Virtual::Thread *thread) {
    m_mutex.lock(); // immediately lock now
    if (!pending()) return m_mutex.unlock();

    // prepare the callback to be used now
    Switch::Callback callback = [&](Virtual::Thread *thread, Virtual::Processor *) {
      $_ASSERT(m_mutex.state(), "Expected deferred to be locked");
      $_ASSERT(m_queue != nullptr, "Expected deferred to be pending");
      m_queue = m_append(m_queue, thread), m_mutex.unlock();
    };

    // schedule the deferred for waiting now
    Switch::Context::yield_to_scheduler(thread, Virtual::State::WAITING, std::move(callback));
  }

  /**
   * @brief Notifies that the result has changed.
   * @param thread                Virtual thread.
   */
  inline void m_notify(Virtual::Thread *thread) {
    $_ASSERT(m_mutex.state(), "Expected deferred to be locked");
    auto *processor = thread->worker()->processor(); // get processor

    // attempt scheduling each of the queued threads
    for (size_t ii = 0; ii < m_queue->used; ++ii) {
      auto *sleeping = m_queue->buffer[ii]; // get thread
      sleeping->awaken(), processor->schedule(sleeping);
    }

    // remove the queue now
    m_queue = m_release(m_queue);
  }

  /**
   * @brief Allocates a deferred queue.
   * @param capacity              Capacity to bind.
   */
  static Queue *m_allocate(size_t capacity = 4) {
    size_t size = sizeof(Queue) + $::Memory::Size::PTR * capacity;
    auto *queue = static_cast<Queue *>(std::malloc(size));
    return queue->capacity = capacity, queue->used = 0, queue;
  }

  /**
   * @brief Appends a deferred queue.
   * @param self                  Queue to append.
   * @param thread                Thread to append.
   */
  static Queue *m_append(Queue *self, Virtual::Thread *thread) {
    $_ASSERT(self->used <= self->capacity, "Exceeded Deferred::Queue capacity"); // ensure valid
    if (self->used != self->capacity) return self->buffer[self->used] = thread, ++self->used, self;

    auto *next = m_allocate(self->capacity * 2); // allocate the next capacity value
    for (size_t ii = 0; ii < self->used; ++ii) m_append(next, self->buffer[ii]);
    return m_append(next, thread), std::free(self), next; // and append this thread
  }

  /**
   * @brief Releases a deferred queue.
   * @param self                  Queue to release.
   */
  static Queue *m_release(Queue *self) {
    if (self != nullptr) std::free(self);
    return nullptr; // return empty now
  }
};

} // namespace XSIO::Task

#endif
