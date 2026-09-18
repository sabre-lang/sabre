#ifndef _SABRE_DYLIB_ADDON_HPP
#define _SABRE_DYLIB_ADDON_HPP

/// Sabre Includes
#include "sabre/forward/dylib.hpp"

namespace Sabre::Dylib {

/// @brief Library Addon Container.
class Addon {
  //  PROPERTIES  //

  /// @brief Underlying loader.
  Loader m_loader = nullptr;

  /// @brief Bound export values.
  Value::Any m_exports = Value::Void();

  /// @brief Associated addon name.
  $::String::Buffer m_name = {};

  /// @brief Base mutex value to use.
  mutable $::Mutex::Auto m_mutex;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an empty addon.
   * @param name                  Name of addon.
   * @param loader                Loader to inherit.
   */
  constexpr Addon(const $::String::View &name, Loader &&loader = nullptr) : m_loader(std::move(loader)), m_name(name) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the name of an addon.
  inline constexpr $::String::View name() const noexcept { return m_name; }

  /// @brief Gets the underlying addon exports.
  inline constexpr Value::Any &exports() noexcept { return m_exports; }
  inline constexpr Value::Any exports() const noexcept { return m_exports; }

  /// @brief Denotes if the addon is currently loaded.
  inline constexpr bool ready() const noexcept { return !m_exports.is<Value::Void>(); }

  /**
   * @brief Handles loading dynamic libraries.
   * @param isolate               Thread isolate.
   */
  inline constexpr Value::Any preload(Runtime::Isolate *isolate) { return m_preload(isolate); }

  /**
   * @brief Handles unloading dynamic libraries.
   * @param isolate               Thread isolate.
   */
  inline constexpr void unload(Runtime::Isolate *isolate) { return m_unload(isolate); }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles loading dynamic libraries.
   * @param isolate               Thread isolate.
   */
  Value::Any m_preload(Runtime::Isolate *isolate);

  /**
   * @brief Handles unloading dynamic libraries.
   * @param isolate               Thread isolate.
   */
  void m_unload(Runtime::Isolate *isolate);
};

} // namespace Sabre::Dylib

#endif
