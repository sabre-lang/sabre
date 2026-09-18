#ifndef _SABRE_DYLIB_REGISTRY_HPP
#define _SABRE_DYLIB_REGISTRY_HPP

/// Sabre Includes
#include "sabre/dylib/mixin.hpp"
#include "sabre/globals/roots.hpp"
#include "sabre/lifecycle/service.hpp"

namespace Sabre::Dylib {

/// @brief Library Registry.
class Registry : public $::Ensure::Singleton<Registry> {
  //  PROPERTIES  //

  /// @brief Storage mutex value.
  mutable $::Mutex::Auto m_mutex;

  /// @brief All available dynamic libraries.
  $::Map::Record<$::Unique::Pointer<Addon>> m_addons = {};

public:
  //  PUBLIC METHODS  //

  /// @brief Gets the total addons that have been installed.
  static inline constexpr size_t total() noexcept { return s_instance()->m_addons.size(); }

  /**
   * @brief Denotes if a library has been bound.
   * @param name                      Name of library.
   */
  static inline constexpr bool bound(const $::String::View &name) { return s_instance()->m_addons.contains(name); }

  /**
   * @brief Allows iterating over the addon roots.
   * @param yield                     Globals cache.
   */
  static inline constexpr void each(Globals::Each &yield) noexcept {
    for (const auto &addon : s_instance()->m_addons | std::views::values) yield(addon->exports());
  }

  /**
   * @brief Handles installing a dynamic library.
   * @param addon                     Library addon.
   */
  static Addon *install($::Unique::Pointer<Addon> &&addon);

  /// @brief Handles installing a dynamic package.
  template <class T> static inline Addon *install() {
    return install(T::m_name(), [](Runtime::Isolate *isolate, Exports &exports) {
      auto *package = new T(isolate, exports); // prepare the addon
      m_lifecycle(isolate)->subscribe([package] { delete package; });
    });
  }

  /**
   * @brief Handles installing a dynamic library.
   * @param name                      Name of library.
   * @param loader                    Loader to bind.
   */
  static inline Addon *install(const $::String::View &name, Loader &&loader) {
    return install($::Unique::New<Addon>(name, std::move(loader)));
  }

  /**
   * @brief Handles loading dynamic libraries.
   * @param isolate                   Thread isolate.
   * @param name                      Library name.
   */
  static Value::Any preload(Runtime::Isolate *isolate, const $::String::View &name);

  /**
   * @brief Forces a dynamic library to be unloaded.
   * @param isolate                   Thread isolate.
   * @param name                      Library name.
   */
  static void unload(Runtime::Isolate *isolate, const $::String::View &name);

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles getting the underlying lifecycle.
   * @param isolate                   Thread isolate.
   */
  static Lifecycle::Service *m_lifecycle(Runtime::Isolate *isolate);
};

} // namespace Sabre::Dylib

#endif
