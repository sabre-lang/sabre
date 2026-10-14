/// Sabre Includes
#include "sabre/author/docify.hpp"
#include "sabre/author/serializer.hpp"
#include "sabre/crate/registry.hpp"
#include "sabre/import/service.hpp"
#include "sabre/relint/mirror.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/type/metadata.hpp"

//  CONSTRUCTORS  /

Sabre::Author::Docify::Docify(const Options &options) : Docify($::Global::get<Runtime::Container>(), options) {}
Sabre::Author::Docify::Docify(XI::Container *services, const Options &options) :
    m_options(options), m_services(services), m_modules(m_services->get<Import::Service>()->storage()) {}

//  PRIVATE METHODS  //

void Sabre::Author::Docify::m_initialize(const $::URI::View &resource) {
  // prepare the basic project to be used
  m_project = $::Unique::New<Project>();

  // bind the baseline resource being used
  m_project->resource = resource;

  // and being processing the input resource
  m_process(resource);
}

int32_t Sabre::Author::Docify::m_finalize(const $::URI::View &resource) {
  // find a potential parent crate being used
  auto crate = Crate::Registry::scan(resource);

  // check if the crate has the correct entry
  if (crate && crate->main() != resource) crate = nullptr;

  // ensure we have a suitable title to be assigned now
  if (m_options.title.has_value()) m_project->title = *m_options.title;
  else if (crate == nullptr) m_project->title = "Documentation";
  else if (auto title = crate->title(); title.size()) m_project->title = title;
  else if (auto name = crate->name(); name.size()) m_project->title = name;

  // bind the baseline description as well now
  if (crate) m_project->description = crate->describe();

  // and serialize based on our details
  return Serializer(this).write(), EXIT_SUCCESS;
}

void Sabre::Author::Docify::m_process(const $::URI::View &resource) {
  // get the incoming module instance
  auto *module = m_modules->lookup(resource);

  // ensure the module actually exists before continuing
  $_ASSERT(module, "Missing module for '{0}'", resource.relative());

  // can safely process the module instance now
  m_process(module);
}

void Sabre::Author::Docify::m_process(const Module::Abstract *module) {
  // get the current metadata for all types
  auto *metadata = module->metadata<Module::Phase::TYPED>();

  // get all the available variables to be potentially emitted
  const auto *references = metadata->mirrors()->references();

  // iterate over the references to attempt emitting them
  for (const auto &[key, definition] : references->view()) {
    if (definition->variable) m_reference(definition->variable);
    if (definition->annotation) m_reference(definition->annotation);
  }
}

void Sabre::Author::Docify::m_reference(const Relint::Mirror *mirror) {
  // resolve a suitable declaration node to be handled
  const auto *node = mirror->when<Syntax::Declaration>();

  // ignore if the node is not suitable for being emitted
  if (node == nullptr || !node->modifiers().test(Variable::Flag::EXPORT)) return;

  // if this declaration already exists, then we ignore it
  if (m_project->storage.contains(node)) return;

  // we need to suitably reflect this node as needed now
  auto declaration = $::Unique::New<Declaration>(node, mirror);
  m_project->storage.emplace(node, std::move(declaration));

  /// TODO: bind to a category or other area
}
