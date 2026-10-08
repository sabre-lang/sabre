/// Sabre Includes
#include "sabre/author/docify.hpp"
#include "sabre/import/service.hpp"
#include "sabre/relint/mirror.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/type/metadata.hpp"

//  CONSTRUCTORS  /

Sabre::Author::Docify::Docify() : Docify($::Global::get<Runtime::Container>()) {}
Sabre::Author::Docify::Docify(XI::Container *services) : m_services(services) {}

//  PUBLIC METHODS  //

int32_t Sabre::Author::Docify::process(const $::URI::View &resource, const Options &) {
  // get the underlying importer service now
  auto *modules = m_services->get<Import::Service>();

  // resolve the head module to be used
  auto *head = modules->storage()->lookup(resource);

  // throw a warning if necessary when missing a resource
  $_ASSERT(head != nullptr, "Expected head documentation module");

  // get the current documentation details now
  auto *metadata = head->metadata<Module::Phase::TYPED>();
  auto *references = metadata->mirrors()->references();

  /// TODO: iterate over the available definitions to be documented
  for ($_UNUSED const auto &[key, definition] : references->view()) {}

  // on completion, declare as a success here
  $_ABORT("Unimplemented 'docify' command");
}
