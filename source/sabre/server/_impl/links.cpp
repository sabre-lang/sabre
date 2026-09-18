/// Sabre Modules
#include "sabre/import/service.hpp"
#include "sabre/server/connection.hpp"

//  EVENT METHODS  //

void Sabre::Server::Events::on_document_links(Request::DOCUMENT_LINKS request) {
  // prepare the links details to be used
  auto resource = request.params().identifier.resource;

  // show that a format was requested
  $_TRACE("--| links: requested '{0}'", resource.relative());

  // prepare an empty response to be returned
  auto response = request.prepare();

  // get the incoming modules service to use
  auto *modules = m_connection->service<Import::Service>();

  // start by resolving the node at the resource/position now
  if (auto *mirrors = m_utilities->syntax_view_at(resource)) {
    // prepare all the available import statements
    auto imports = mirrors->filter<Syntax::Import>();

    // pre-size the response (assuming all imports succeed)
    response.links.reserve(imports.size());

    // iterate over them to collapse into suitable links
    for (const auto *import : imports) {
      auto result = modules->resolve(import->path(), resource.body());
      if (!result.has_value() || result->scheme() != $::URI::Scheme::FILE) continue;
      response.links.emplace_back(XLSP::Anchor(*result, import->trivia()->range().client()));
    }
  }

  // and ensure we reply as expected now
  request.reply(std::move(response));
}
