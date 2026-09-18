#ifndef _SABRE_SERVER_CONNECTION_HPP
#define _SABRE_SERVER_CONNECTION_HPP

/// Sabre Includes
#include "sabre/async/service.hpp"
#include "sabre/server/events.hpp"
#include "sabre/server/options.hpp"
#include "sabre/server/transport.hpp"
#include "sabre/server/utilities.hpp"
#include "sabre/server/worker.hpp"

namespace Sabre::Server {

/// @brief LSP Connection Service.
class Connection : public XI::Singleton, public XLSP::Server::Connection {
  //  PROPERTIES  //

  /// @brief The mutex for analysis.
  mutable $::Mutex::Auto m_mutex;

  /// @brief Available services container.
  XI::Container *m_services;

  /// @brief Asynchrononous runtime service.
  Async::Service *m_async;

  /// @brief Events dispatcher instance.
  $::Unique::Pointer<Events> m_events;

  /// @brief Helpert utilities to be used.
  $::Unique::Pointer<Utilities> m_utilities;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a server connection.
   * @param services              Services container.
   */
  explicit Connection();
  explicit Connection(XI::Container *services);

  //  PUBLIC METHODS  //

  /// @brief Prepares a suitable scoped connection guard.
  $_NODISCARD inline constexpr auto guard() noexcept { return $::Lock::guard(m_mutex); }

  /// @brief Gets the underlying utilities.
  inline constexpr const Utilities *utilities() const noexcept { return m_utilities.get(); }

  /// @brief Allows getting underlying services.
  template <std::derived_from<XI::Singleton> T> inline constexpr T *service() const noexcept { return *m_services; }
  template <std::derived_from<XI::Transient> T> inline constexpr auto service() const noexcept { return *m_services; }

  /**
   * @brief Handles scheduling workers.
   * @param args                  Worker arguments.
   */
  template <class... As> inline constexpr void schedule(As &&...args) {
    m_async->spawn<Worker>(this, std::forward<As>(args)...);
  }

  /**
   * @brief Handles coordinating full analysis.
   * @param args                  Worker arguments.
   */
  template <class... As> inline constexpr void analyze(As &&...args) {
    analyze(Refresh::ENTIRE, std::forward<As>(args)...);
  }

  /**
   * @brief Handles coordinating analysis.
   * @param mode                  Refersh mode.
   * @param args                  Worker arguments.
   */
  template <class... As> inline constexpr void analyze(Refresh mode, As &&...args) {
    schedule(std::forward<As>(args)..., [mode](Worker *worker) { worker->analyze(mode); });
  }

private:
  //  PRIVATE METHODS  //

  /// @brief Handles launching the connection instance.
  int32_t m_listen() final;
};

} // namespace Sabre::Server

#endif
