/// Sabre Modules
#include "sabre/server/connection.hpp"

//  EVENT METHODS  //

void Sabre::Server::Events::on_document_symbols(Request::DOCUMENT_SYMBOLS request) {
  // symbols requires modules to be ready
  $_UNUSED $_AUTO = m_connection->guard();

  // destructure the incoming items to be used
  auto resource = request.params().identifier.resource;

  // show that a message was requested
  $_TRACE("--| symbols: requested '{0}'", resource.relative());

  // prepare the symbols response to be used
  auto response = request.prepare();

  // only bind the symbols if possible to do so
  auto *mirrors = m_utilities->syntax_view_at(resource);
  if (mirrors) response.symbols = mirrors->symbols();

  // and ensure we reply as expected now
  request.reply(std::move(response));
}
