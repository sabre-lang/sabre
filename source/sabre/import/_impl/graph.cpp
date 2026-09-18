/// Sabre Includes
#include "sabre/import/graph.hpp"
#include "sabre/async/service.hpp"
#include "sabre/async/thenable.hpp"
#include "sabre/import/service.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/runtime/isolate.hpp"

//  CONSTRUCTORS  //

Sabre::Import::Graph::Graph(const Dependencies &dependencies) :
    Graph($::Global::get<Runtime::Container>(), dependencies) {}

Sabre::Import::Graph::Graph(XI::Container *services, const Dependencies &dependencies) :
    m_services(services), m_modules(*m_services), m_async(*m_services) {
  for (const auto &resource : dependencies) queue(resource);
}

//  PUBLIC METHODS  //

std::vector<Sabre::Module::Abstract *> Sabre::Import::Graph::roots() const noexcept {
  auto predicate = [](const Nodule *node) { return node->module; };
  return $::Ranges::To(m_roots | std::views::transform(predicate));
}

std::vector<Sabre::Module::Abstract *> Sabre::Import::Graph::leaves() const noexcept {
  // prepare the resultant values to be used
  auto modules = std::vector<Module::Abstract *>();

  // we start with a visited set that includes empty modules (to skip)
  auto visited = $::Map::Set<const Module::Abstract *>({nullptr});

  // construct a queue filled with the underlying nodules
  auto queued = std::queue<const Nodule *>(m_roots.begin(), m_roots.end());

  // attempt a "breadth-first-search" now
  for (; queued.size(); queued.pop()) {
    // get the top-most node to work with now
    auto *head = queued.front();

    // emplace all the children whilst possible to do so
    for (const auto *child : head->children) {
      if (!visited.contains(child->module)) queued.push(child);
    }

    // emplace this node onto the visited stack and module list
    modules.emplace_back(head->module), visited.emplace(head->module);
  }

  // return the resulting leaves now
  return modules;
}

const Sabre::Import::Nodule *Sabre::Import::Graph::queue(Module::Abstract *module) { return m_link(module, nullptr); }
const Sabre::Import::Nodule *Sabre::Import::Graph::queue(const $::URI::Buffer &resource) {
  auto result = Result();
  m_import(&result, resource);
  return queue(m_await(&result));
}

//  PRIVATE METHODS  //

Sabre::Import::Nodule *Sabre::Import::Graph::m_link(Module::Abstract *module, Nodule *parent) {
  // ignore if the module does not actually exist
  if ($_UNLIKELY(module == nullptr)) return nullptr;

  // check if the module already exists
  auto iter = m_nodes.find(module);
  if (iter != m_nodes.cend()) return iter->second.get();

  // construct the new nodule instance now
  auto *node = m_nodes.emplace(module, $::Unique::New<Nodule>(module)).first->second.get();

  // update the nodules parent details now
  if (parent != nullptr) node->parents.emplace_back(parent), parent->children.emplace_back(node);

  // attempt scanning across the available depedencies
  for (const auto &dependency : m_import(module)) m_link(dependency, node);

  // resolve whether or not the node should be a root
  node->parents.size() ? $_PP_IGN(m_roots.erase(node)) : $_PP_IGN(m_roots.emplace(node));

  // and return the resulting nodule instance as needed
  return node;
}

Sabre::Module::Abstract *Sabre::Import::Graph::m_await(Result *deferred) {
  auto *thread = m_async->isolate()->thread();
  auto result = deferred->await(thread);
  return result.has_value() ? *result : nullptr;
}

void Sabre::Import::Graph::m_import(Result *deferred, const $::URI::Buffer &resource) {
  m_async->spawn<Worker>(m_services, deferred, resource);
}

std::vector<Sabre::Module::Abstract *> Sabre::Import::Graph::m_import(const Dependencies &resources) {
  // stop early if there are no resources given
  if (resources.empty()) return {};

  // prepare some output containers to be used now
  auto modules = std::vector<Module::Abstract *>(resources.size());
  auto workers = std::vector<$::Unique::Pointer<Result>>(resources.size());

  // bind all the workers to their imports as needed
  for (const auto &[ii, resource] : $::Ranges::Each(resources)) {
    m_import((workers[ii] = $::Unique::New<Result>()).get(), resource);
  }

  // and await all the workers in unison afterwards
  for (size_t ii = 0; ii < resources.size(); ++ii) modules[ii] = m_await(workers[ii].get());

  // return all found dependencies now
  return modules;
}

std::vector<Sabre::Module::Abstract *> Sabre::Import::Graph::m_import(Sabre::Module::Abstract *parent) {
  auto resources = Dependencies();
  auto hint = parent->resource().body();
  auto dependencies = parent->dependencies();

  // pre-fill the resources sizing as well
  resources.reserve(dependencies.size());

  // attempt iterating over the available module resources
  for (const auto &path : dependencies) {
    auto resource = m_modules->resolve(path, hint);
    if (resource) resources.emplace_back(*resource);
  }

  // and return the final result of dependencies
  return m_import(resources);
}
