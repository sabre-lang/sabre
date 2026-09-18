/// Sabre Includes
#include "sabre/server/connection.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/runtime/options.hpp"

//  CONSTRUCTORS  //

Sabre::Server::Connection::Connection() : Connection($::Global::get<Runtime::Container>()) {}
Sabre::Server::Connection::Connection(XI::Container *services) :
    XLSP::Server::Connection(Options()), m_services(services), m_async(*m_services), m_utilities(*services) {}

//  PRIVATE METHODS  //

int32_t Sabre::Server::Connection::m_listen() {
  // prepare the events seperately since it requires different items
  auto events = (m_events = m_services->get<Events>(this)).get();

  // prepare an initial binder to be used
  auto bind = binder();

  // and prepare the base lifecycle events to be handled
  bind.on_request(events, &Events::on_initialize);
  bind.on_request(events, &Events::on_shutdown);

  // and finally start listening to the transport
  return m_async->launch<Transport>(this);
}
