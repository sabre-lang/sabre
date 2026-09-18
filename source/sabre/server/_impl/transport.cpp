/// Sabre Includes
#include "sabre/server/transport.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/server/connection.hpp"

//  CONSTRUCTORS  //

Sabre::Server::Transport::Transport() : Transport($::Global::get<Runtime::Container>()) {}
Sabre::Server::Transport::Transport(XI::Container *services) : Transport(*services) {}
Sabre::Server::Transport::Transport(Connection *connection) : m_connection(connection), m_emitter(connection) {}

//  PRIVATE METHODS  //

$_NORETURN void Sabre::Server::Transport::m_execute() {
  m_thread->shutdown(m_connection->transport()->listen(&m_emitter));
}
