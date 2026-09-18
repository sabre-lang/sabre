/// Sabre Modules
#include "sabre/toolchain/format.hpp"
#include "sabre/server/connection.hpp"

//  EVENT METHODS  //

void Sabre::Server::Events::on_document_format(Request::DOCUMENT_FORMAT request) {
  // create a copy of the resource since we move the request
  auto resource = request.params().identifier.resource;

  // and attempt requesting the formatter when available to work
  m_connection->schedule(resource, [request = std::move(request)](Worker *worker) {
    auto response = request.prepare();
    response.edits = worker->format();
    request.reply(std::move(response));
  });
}
