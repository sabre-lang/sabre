/// Sabre Modules
#include "sabre/server/connection.hpp"

//  EVENT METHODS  //

void Sabre::Server::Events::on_document_completes(Request::DOCUMENT_COMPLETIONS request) {
  // since this relies on an updated set of modules, we guard the connection
  $_UNUSED $_AUTO = m_connection->guard();

  // destructure the incoming items to be used
  auto cursor = request.params();
  auto position = cursor.position;
  auto resource = cursor.identifier.resource;

  // show that a message was requested
  $_TRACE("--| completions: requested '{0}:{1}'", resource.relative(), position);

  // prepare the response to be used
  auto response = request.prepare();

  // resolve the incoming node that is being completed (expecting an identifier)
  if (auto *node = m_utilities->syntax_node_at<Syntax::Identifier>(resource, position)) {
    // show that we found a node that we could use for our completions handler now
    $_TRACE("--| completions: found node '{0}' at {1}", node->canonical(), node->range());
  }

  // and ensure we reply as expected now
  request.reply(std::move(response));
}
