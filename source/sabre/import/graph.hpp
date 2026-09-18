#ifndef _SABRE_IMPORT_GRAPH_HPP
#define _SABRE_IMPORT_GRAPH_HPP

/// Sabre Includes
#include "sabre/forward/syntax.hpp"
#include "sabre/import/worker.hpp"

namespace Sabre::Import {

/// @brief Outgoing Import Statistics.
struct Statistics {
  size_t errors = 0;
  size_t hints = 0;
};

/// @brief Explicit Graph Dependencies.
using Dependencies = std::vector<$::URI::Buffer>;

/// @brief A graph nodule.
struct Nodule {
  //  PROPERTIES  //

  /// @brief Associated module reference.
  Module::Abstract *module = nullptr;

  /// @brief Parent module reference.
  std::vector<Nodule *> parents = {};

  /// @brief Dependencies bound to a nodule.
  std::vector<Nodule *> children = {};

  //  CONSTRUCTORS  //

  /// @brief Allow default construction (for root nodes).
  constexpr Nodule() = default;

  /**
   * @brief Allow construction of nodules.
   * @param module            Module interface.
   */
  constexpr Nodule(Module::Abstract *module) : module(module) {}
};

/// @brief Module Dependency Graph.
class Graph : public XI::Transient {
  //  PROPERTIES  //

  /// @brief Services container.
  XI::Container *m_services;

  /// @brief Bound modules service.
  Service *m_modules;

  /// @brief Asynchronous service.
  Async::Service *m_async;

  /// @brief All independent graph roots.
  $::Map::Set<Nodule *> m_roots = {};

  /// @brief All bound graph nodes.
  $::Map::Base<Module::Abstract *, $::Unique::Pointer<Nodule>> m_nodes = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a module graph.
   * @param services                Services container.
   * @param dependencies            Dependencies to bind.
   */
  explicit Graph(const Dependencies &dependencies = {});
  explicit Graph(XI::Container *services, const Dependencies &dependencies = {});

  //  PUBLIC METHODS  //

  /// @brief Gets all the available root nodes.
  std::vector<Module::Abstract *> roots() const noexcept;

  /// @brief Gets all the available trailing nodes.
  std::vector<Module::Abstract *> leaves() const noexcept;

  /**
   * @brief Handles adding modules to the graph.
   * @param resource                Module to add.
   */
  const Nodule *queue(Module::Abstract *module);
  const Nodule *queue(const $::URI::Buffer &resource);

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles linking a depedency to a parent.
   * @param dependency                Depedency to link.
   * @param parent                    Parent nodule reference.
   */
  Nodule *m_link(Module::Abstract *dependency, Nodule *parent);

  /**
   * @brief Waits for an import result.
   * @param deferred                  Deferred module.
   */
  Module::Abstract *m_await(Result *deferred);

  /**
   * @brief Constructs an asynchronous worker.
   * @param deferred                  Deferred module.
   * @param resource                  Resource to import.
   */
  void m_import(Result *deferred, const $::URI::Buffer &resource);

  /**
   * @brief Handles importing a set of module resources.
   * @param module                    Imports a modules dependencies.
   */
  std::vector<Module::Abstract *> m_import(Module::Abstract *parent);

  /**
   * @brief Handles importing a set of resources.
   * @param deferred                  Bound deferred result.
   * @param resources                 Dependency resources.
   */
  std::vector<Module::Abstract *> m_import(const Dependencies &resources);
};

} // namespace Sabre::Import

#endif
