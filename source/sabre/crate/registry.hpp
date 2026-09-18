#ifndef _SABRE_CRATE_REGISTRY_HPP
#define _SABRE_CRATE_REGISTRY_HPP

/// Sabre Includes
#include "sabre/crate/manifest.hpp"

namespace Sabre::Crate::Registry {

/// @brief Gets the global options that are available.
$::Unique::Pointer<Manifest> global();

/**
 * @brief Handles scanning for parent crates.
 * @param hint                      Hint to use.
 */
$::Unique::Pointer<Manifest> scan(const $::URI::View &hint);
$::Unique::Pointer<Manifest> scan(const $::FS::Path &hint = $::System::cwd());

/**
 * @brief Handles resolving crate package options.
 * @param file_path                 File path.
 */
$::Unique::Pointer<Manifest> view(const $::URI::View &resource);
$::Unique::Pointer<Manifest> view(const $::FS::Path &file_path);

} // namespace Sabre::Crate::Registry

#endif
