#ifndef _SABRE_SHAPE_LOOKUP_HPP
#define _SABRE_SHAPE_LOOKUP_HPP

/// Sabre Includes
#include "sabre/shape/limits.hpp"

namespace Sabre::Shape {

/// @brief Invalid Shape Value.
static constexpr Underlying Invalid() { return Limits::MAXIMUM; }

/// @brief Handles looking up associated shapes.
template <class T> static constexpr Underlying Lookup() {
  // prepare some fast lookups for strings (since no overlap for immediate/object)
  if constexpr (std::same_as<String::Small, T>) return Lookup<String::Any>();
  if constexpr (std::same_as<String::Large, T>) return Lookup<String::Any>();

  // prepare an initial value on the threshold
  Underlying shape = Limits::THRESHOLD;

#define X(V, ...)                                  \
  case $::RTTI::Hash<V>(): --shape; $_FALLTHROUGH;
  switch ($::RTTI::Hash<T>()) {
    SABRE_XX_VALUES_IMMEDIATE(X)
    SABRE_XX_VALUES_OBJECT(X)
    X(Object::Instance)

  // and resolve as necessary now
  default: return shape == Limits::THRESHOLD ? Invalid() : shape;
  }
#undef X
}

} // namespace Sabre::Shape

#endif
