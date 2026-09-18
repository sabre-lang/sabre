#ifndef _SABRE_CRATE_SERVICE_HPP
#define _SABRE_CRATE_SERVICE_HPP

/// Sabre Includes
#include "sabre/crate/constants.hpp"
#include "sabre/crate/manifest.hpp"
#include "sabre/value/common.hpp"

namespace Sabre::Crate {

/// @brief Crate Runtime Service.
class Service : public XI::Singleton {
  //  PROPERTIES  //

  /// @brief Crates mutex.
  mutable $::Mutex::Auto m_mutex;

  /// @brief All resolved crates.
  $::Map::Dict<$::Unique::Pointer<Manifest>> m_storage = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Allow default construction.
  explicit Service() = default;

  //  PUBLIC METHODS  //

  /**
   * @brief Handles validating file-paths.
   * @param resource                  Resource to resolve.
   */
  bool validate(const $::FS::Path &resource);

  /**
   * @brief Handles getting a cached crate resource package.
   * @param resource                  Resource to resolve.
   */
  const Manifest *resolve(const $::FS::Path &resource);
  const Manifest *resolve(const $::URI::View &resource);

  /**
   * @brief Handles getting dynamic crate values.
   * @param isolate                   Thread isolate.
   * @param name                      Library name.
   */
  Value::Any dylib(Runtime::Isolate *isolate, const $::String::View &name) const noexcept;
};

} // namespace Sabre::Crate

#endif
