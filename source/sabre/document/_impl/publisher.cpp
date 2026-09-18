/// Sabre Includes
#include "sabre/document/publisher.hpp"
#include "sabre/import/service.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/server/connection.hpp"

//  CONSTRUCTORS  //

Sabre::Document::Publisher::Publisher() : Publisher($::Global::get<Runtime::Container>()) {}
Sabre::Document::Publisher::Publisher(XI::Container *services) :
    m_services(services), m_connection(services->when<Server::Connection>()) {}

//  PUBLIC METHODS  //

void Sabre::Document::Publisher::clear(const $::URI::View &resource) {
  publish(resource, {}); // remove resource here now as needed
}

void Sabre::Document::Publisher::refresh(const $::URI::View &resource) {
  if (auto *modules = m_modules()) modules->storage()->remove(resource);
}

void Sabre::Document::Publisher::refresh(const std::vector<$::URI::View> &resources) {
  for (const auto &resource : resources) refresh(resource);
}

void Sabre::Document::Publisher::refresh(const std::vector<$::URI::Buffer> &resources) {
  for (const auto &resource : resources) refresh(resource);
}

void Sabre::Document::Publisher::publish(
    const $::URI::View &resource, const std::vector<XLSP::Diagnostic> &diagnostics
) {
  using Notification = Server::Notification::DIAGNOSTICS_PUBLISH; // prepare typing
  if (m_connection) m_connection->notify(Notification(resource.buffer(), diagnostics));
}

//  PRIVATE METHODS  //

Sabre::Import::Service *Sabre::Document::Publisher::m_modules() noexcept {
  return m_services && m_connection ? m_services->get<Import::Service>() : nullptr;
}
