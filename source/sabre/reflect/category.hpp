#ifndef _SABRE_REFLECT_CATEGORY_HPP
#define _SABRE_REFLECT_CATEGORY_HPP

/// Sabre Includes
#include "sabre/forward/reflect.hpp"

//  X-MACROS  //

#define SABRE_XX_REFLECTION_FEATURES(X) X(JITTED, "Optimize")

#define SABRE_XX_REFLECTION_CATEGORIES(X) \
  X(INTERNAL, Internal, "Internal")       \
  X(OVERLOAD, Overload, "Overload")       \
  X(OPERATOR, Override, "Operator")       \
  X(DEPRECATED, Deprecated, "Deprecated")

//  NAMESPACES  //

namespace Sabre::Reflect {

/// @brief Available Compile-Time Features.
$_XX_ENUM_CLASS(Feature, uint8_t, SABRE_XX_REFLECTION_FEATURES, INVALID = UINT8_MAX);

/// @brief Available Compile-Time Categories.
$_XX_ENUM_CLASS(Category, uint8_t, SABRE_XX_REFLECTION_CATEGORIES, INVALID = UINT8_MAX);

} // namespace Sabre::Reflect

#endif
