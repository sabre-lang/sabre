#ifndef _SABRE_PRODUCT_DETAILS_HPP
#define _SABRE_PRODUCT_DETAILS_HPP

/// Sabre Includes
#include "sabre/product/semver.hpp"
#include "sabre/vendors/versions.hpp"

namespace Sabre::Product {

//  PUBLIC METHODS  //

/// @brief Gets the product title.
static inline constexpr $::String::View title() { return SABRE_MM_TITLE; }

/// @brief Gets the product identifier.
static inline constexpr $::String::View identifier() { return SABRE_MM_IDENTIFIER; }

/// @brief Gets the executable version.
static inline constexpr Version::Semantic version() {
  static auto s_version = Version::Semantic(SABRE_MM_VERSION);
  return s_version; // and return the resulting version now
}

/// @brief Gets all the available vendors.
static inline constexpr $::Map::Record<Version::Semantic> vendors() {
#define X(N, V, ...) {N, Version::Semantic(V)},
  static $::Map::Record<Version::Semantic> s_vendors = {SABRE_XX_VENDOR_VERSIONS(X)};
  return s_vendors; // and return the result now
#undef X
}

/// @brief Gets the available scripts path.
static inline constexpr $::FS::Path scripts() {
  static auto s_parent = $::Path::canonical($::Executable::dirname() / ".." / "scripts");
  return s_parent; // resolve the scripts location now
}

} // namespace Sabre::Product

#endif
