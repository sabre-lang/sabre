#ifndef _SABRE_CRATE_CONSTANTS_HPP
#define _SABRE_CRATE_CONSTANTS_HPP

/// Sabre Includes
#include "sabre/forward/crate.hpp"

namespace Sabre::Crate::Constants {

//  PROPERTIES  //

/// @brief Explicit crate filename.
static inline constexpr $::String::View filename() { return "_crate.jsonc"; }

//  PUBLIC METHODS  //

/// @brief Gets the builtin crates directory.
static inline constexpr $::FS::Path internal() { return $::Path::canonical("../crates", $::Executable::resolve()); }
static inline constexpr $::FS::Path internal(const $::String::View &name) { return internal() / name / filename(); }

/// @brief Gets the vendors crates directory.
static inline constexpr $::FS::Path external() { $_ABORT("Unimplemented!"); }
static inline constexpr $::FS::Path external(const $::String::View &name) { return external() / name / filename(); }

} // namespace Sabre::Crate::Constants

#endif
