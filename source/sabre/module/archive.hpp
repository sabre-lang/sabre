#ifndef _SABRE_MODULE_BINARY_HPP
#define _SABRE_MODULE_BINARY_HPP

/// Sabre Includes
#include "sabre/forward/image.hpp"
#include "sabre/module/abstract.hpp"

namespace Sabre::Module {

/// @brief Bundled Archive Module.
struct Archive : public Mixin<Archive> {
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructors.
  using Mixin::Mixin;

  /**
   * @brief Constructs a module from a compilation archive.
   * @param arena                     Archive arena.
   */
  explicit Archive($::Unique::Pointer<Image::Arena> &&arena);
  explicit Archive(XI::Container *services, $::Unique::Pointer<Image::Arena> &&arena);
};

} // namespace Sabre::Module

#endif
