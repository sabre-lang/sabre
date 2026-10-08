#ifndef _XLSP_EVENT_STORAGE_HPP
#define _XLSP_EVENT_STORAGE_HPP

/// XLSP Includes
#include "xlsp/forward/event.hpp"
#include "xlsp/forward/server.hpp"
#include "xlsp/message/request.hpp"

namespace XLSP::Event {

/// @brief Callback Answer Callback.
using Answer = $::Unique::Functor<void(const Message::Result<$::Serde::Value> &) const>;

/// @brief Event Storage Container.
class Storage {
  //  TYPEDEFS  //

  /// @brief Allow the server internal access.
  friend class Server::Connection;

  /// @brief Function Collection Mapping.
  template <class F> using Collection = $::Map::View<$::Unique::Functor<F>>;

  /// @brief Pending Response Typing.
  struct Pending {
    //  PROPERTIES  //

    /// @brief Bound answer value.
    Answer answer;

    /// @brief Response identifier.
    $::String::Buffer identifier;

    //  CONSTRUCTORS  //

    /**
     * @brief Constructs a pending response.
     * @param identifier                Response identifier.
     * @param answer                    Answer to formulate.
     */
    constexpr Pending(const $::String::Buffer &identifier, Answer &&answer) :
        answer(std::move(answer)), identifier(identifier) {}

    //  OPERATOR METHODS  //

    /// @brief Explicit operators available.
    inline constexpr bool operator==(const Pending &other) const noexcept { return identifier == other.identifier; }
    inline constexpr bool operator!=(const Pending &other) const noexcept { return identifier != other.identifier; }
  };

public:
  //  PROPERTIES  //

  /// @brief Current call-identifier.
  int64_t identifier = 0;

  /// @brief Available event mutexes.
  struct {
    mutable $::Mutex::Auto reply;  // Response mutex.
    mutable $::Mutex::Auto cancel; // Cancellation mutex.
  } mutex;

  /// @brief Pending response callbacks.
  std::deque<Pending> responses = {};

  /// @brief Currently pending cancellations.
  $::Map::Set<$::String::Buffer> cancellations = {};

  /// @brief Currently bound notifications.
  Collection<void(const $::Serde::Value &) const> notifications = {};

  /// @brief Currently bound requests.
  Collection<void(const $::Serde::Value &, Answer &&) const> requests = {};

  //  CONSTRUCTORS  //

  /// @brief Constructs a defaulted cache instance.
  constexpr Storage() = default;
};

} // namespace XLSP::Event

#endif
