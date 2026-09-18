/// Sabre Includes
#include "sabre/server/events.hpp"
#include "sabre/document/service.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/server/connection.hpp"

//  CONSTRUCTORS  //

Sabre::Server::Events::Events() : Events($::Global::get<Runtime::Container>()) {}
Sabre::Server::Events::Events(XI::Container *services) : Events(services, *services) {}
Sabre::Server::Events::Events(XI::Container *services, Connection *connection) :
    m_connection(connection), m_utilities(m_connection->utilities()), m_documents(*services) {}
