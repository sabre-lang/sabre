#ifndef _SABRE_LOCALE_OPTIONS_HPP
#define _SABRE_LOCALE_OPTIONS_HPP

/// Sabre Includes
#include "sabre/forward/locale.hpp"

namespace Sabre::Locale {

/// @brief Locale Options.
struct Options {
  //  PROPERTIES  //

  /// @brief Denotes the initial locale.
  $::String::Buffer initial = {};

  //  CONSTRUCTORS  //

  /// @brief Default locale options constructor.
  constexpr Options() = default;
};

} // namespace Sabre::Locale

#endif
