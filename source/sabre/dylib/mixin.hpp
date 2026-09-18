#ifndef _SABRE_DYLIB_MIXIN_HPP
#define _SABRE_DYLIB_MIXIN_HPP

/// Sabre Includes
#include "sabre/dylib/addon.hpp"

namespace Sabre::Dylib {

/// @brief Name String Literal.
template <size_t N> using Name = $::String::Literal<N>;

/// @brief Dynamic Library Mixin.
template <Name N> struct $_ABSTRACT Mixin {
  //  TYPEDEFS  //

  /// @brief Allow the registry internal access.
  friend class Registry;

  //  CONSTRUCTORS  //

  /// @brief Default mixin constructor.
  constexpr Mixin() = default;

protected:
  //  PRIVATE METHODS  //

  /// @brief Gets the name of the addon.
  static inline constexpr $::String::View m_name() noexcept { return N.view(); }
};

} // namespace Sabre::Dylib

#endif
