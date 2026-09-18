/// Sabre Includes
#include "sabre/server/connection.hpp"

//  EVENT METHODS  //

void Sabre::Server::Events::on_workspace_watcher(const Notification::WORKSPACE_FILE_CHANGED &params) {
  if (params.changes.size()) m_connection->analyze(); // force a full analysis to occur now
}
