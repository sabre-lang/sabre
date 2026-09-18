/// Sabre Includes
#include "sabre/document/service.hpp"
#include "sabre/import/service.hpp"
#include "sabre/server/connection.hpp"

//  EVENT METHODS  //

void Sabre::Server::Events::on_document_opened(const Notification::DOCUMENT_OPENED &params) {
  on_document_changed(Notification::DOCUMENT_CHANGED(params.document.identifier()));
}

void Sabre::Server::Events::on_document_closed(const Notification::DOCUMENT_CLOSED &params) {
  $_UNUSED $_AUTO = m_connection->guard();
  m_documents->remove(params.identifier.resource);
}

void Sabre::Server::Events::on_document_changed(const Notification::DOCUMENT_CHANGED &params) {
  // since updating documents we wait for readiness
  $_UNUSED $_AUTO = m_connection->guard();

  // attempt re-constructing the buffer we require now
  auto buffer = m_documents->resolve(params.identifier.resource);
  auto content = $::String::Buffer(buffer->view()); // copy
  auto encoding = m_connection->options()->server.encoding;

  // iteratively update the buffer as necessary
  for (const auto &change : params.changes) change.apply(content, encoding);

  // update the document (this will also reset the drafts module)
  m_documents->update(params.identifier.resource, content);

  // and request a rebuild of the underlying modules from this file
  m_connection->analyze(Refresh::PARTIAL, params.identifier.resource);
}
