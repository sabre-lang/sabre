#ifndef _SABRE_SERVER_TRANSPORT_HPP
#define _SABRE_SERVER_TRANSPORT_HPP

/// Sabre Includes
#include "sabre/async/entry.hpp"
#include "sabre/forward/server.hpp"

namespace Sabre::Server {

/// @brief Transport thread handler.
class Transport : public Async::Entry {
  //  PROPERTIES  //

  /// @brief Core transport instance.
  Connection *m_connection;

  /// @brief Core transport dispatcher.
  XLSP::Event::Emitter m_emitter;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a transport task.
   * @param connection                Server connection.
   */
  explicit Transport();
  explicit Transport(Connection *connection);
  explicit Transport(XI::Container *services);

protected:
  //  PRIVATE METHODS  //

  /// @brief Handles executing the transport thread.
  $_NORETURN void m_execute();
};

} // namespace Sabre::Server

#endif
