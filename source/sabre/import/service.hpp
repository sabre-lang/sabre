#ifndef _SABRE_IMPORT_SERVICE_HPP
#define _SABRE_IMPORT_SERVICE_HPP

/// Sabre Includes
#include "sabre/forward/crate.hpp"
#include "sabre/forward/document.hpp"
#include "sabre/forward/image.hpp"
#include "sabre/import/graph.hpp"
#include "sabre/import/loader.hpp"
#include "sabre/import/storage.hpp"
#include "sabre/resource/scheme.hpp"
#include "sabre/resource/trace.hpp"

namespace Sabre::Import {

/// @brief Import/Modules Service.
class Service : public XI::Singleton {
  //  PROPERTIES  //

  /// @brief Services container.
  XI::Container *m_services;

  /// @brief Crates service available.
  Crate::Service *m_crates;

  /// @brief Documents service available.
  Document::Service *m_documents;

  /// @brief Storage for modules.
  $::Unique::Pointer<Storage> m_storage;

  /// @brief Available loaders to be used.
  $::Map::Record<$::Unique::Pointer<Loader>> m_loaders = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an import service.
   * @param services                Services container.
   */
  explicit Service();
  explicit Service(XI::Container *services);

  //  PUBLIC METHODS  //

  /// @brief Gets the available modules storage.
  inline constexpr Storage *storage() const noexcept { return m_storage.get(); }

  /// @brief Gets the available crates service.
  inline constexpr Crate::Service *crates() const noexcept { return m_crates; }

  /// @brief Gets the available documents service.
  inline constexpr Document::Service *documents() const noexcept { return m_documents; }

  /**
   * @brief Handles fetching modules via their loader.
   * @param resource                  Document resource.
   */
  Module::Abstract *fetch(const $::URI::Buffer &resource);

  /**
   * @brief Handles preloading archived modules with arenas.
   * @param arenas                    Arenas to preload.
   */
  void preload(std::vector<$::Unique::Pointer<Image::Arena>> &&arenas);

  /**
   * @brief Handles resolving runtime paths.
   * @param script                    Script value.
   * @param hint                      Directory hint.
   */
  Resource::Result resolve(const $::URI::Buffer &resource) const;
  Resource::Result resolve(const $::String::View &script, const $::FS::Path &hint = $::System::cwd()) const;

  /**
   * @brief Handles importing a module directly.
   * @param isolate                   Runtime isolate.
   * @param resource                  Document resource.
   * @param trace                     Resource trace.
   */
  Async::Thenable *import(Runtime::Isolate *isolate, const $::URI::Buffer &resource, const Resource::Trace &trace = {});

  /**
   * @brief Handles analyzing resources.
   * @param resource                  Resources to analyze.
   * @param report                    Should report results.
   */
  Statistics analyze(const $::URI::Buffer &resource, bool report = false);
  Statistics analyze(const Dependencies &dependencies, bool report = false);

  /**
   * @brief Handles analyzing resources.
   * @param resource                  Resources to analyze.
   * @param reporter                  Collection reporter.
   * @param display                   Display the results.
   */
  Statistics analyze(const $::URI::Buffer &resource, Diagnostic::Reporter *reporter, bool display = false);
  Statistics analyze(const Dependencies &dependencies, Diagnostic::Reporter *reporter, bool display = false);

  /**
   * @brief Constructs an import graph.
   * @param dependencies              Initial graphing dependencies.
   */
  inline $::Unique::Pointer<Graph> graph(const $::URI::Buffer &resource) { return graph(std::vector({resource})); }
  inline $::Unique::Pointer<Graph> graph(const Dependencies &dependencies = {}) {
    return m_services->get<Graph>(dependencies);
  }

  /**
   * @brief Handles subscribing a loader.
   * @param loader                    Loader to subscribe.
   */
  inline constexpr $::String::View subscribe($::Unique::Pointer<Loader> &&loader) noexcept {
    auto result = m_loaders.try_emplace(loader->scheme(), std::move(loader));
    return result.second ? result.first->first : ""; // empty for invalid key
  }

  /**
   * @brief Handles subscribing a loader.
   * @param args                      Loader arguments.
   */
  template <std::derived_from<Loader> T, class... As>
  inline constexpr $::String::View subscribe(As &&...args) noexcept {
    return subscribe(m_services->get<T>(std::forward<As>(args)...));
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Ensures that a module is prepared once fetched.
   * @param module                    Module to ensure.
   */
  Module::Abstract *m_prepare(Module::Abstract *module) const;

  /**
   * @brief Handles base resolution of modules
   * @param script                    Script to resolve.
   * @param hint                      Optional directory hint.
   */
  Resource::Result m_resolve(const $::String::View &script, const $::FS::Path &hint) const;
};

} // namespace Sabre::Import

#endif
