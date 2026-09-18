#ifndef _SABRE_ASYNC_SERVICE_HPP
#define _SABRE_ASYNC_SERVICE_HPP

/// Sabre Includes
#include "sabre/forward/async.hpp"
#include "sabre/runtime/options.hpp"

namespace Sabre::Async {

/// @brief Asynchronous Event-Loop Service.
class Service : public XI::Singleton, public XSIO::Async::Manager {
  //  TYPEDEFS  //

  /// @brief Allow main isolates internal access.
  friend struct Entry;

  //  PROPERTIES  //

  /// @brief The main runtime isolate.
  Runtime::Isolate *m_isolate = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Allow default construction.
  explicit Service() = default;

  /**
   * @brief Constructs an asynchronous manager.
   * @param services          Services container.
   */
  explicit Service(XI::Container *services) : Service(*services) {}
  explicit Service(const Runtime::Options *options) : Manager(&options->async) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the main-runtime isolate.
  inline constexpr Runtime::Isolate *isolate() const noexcept { return m_isolate; }
};

} // namespace Sabre::Async

#endif
