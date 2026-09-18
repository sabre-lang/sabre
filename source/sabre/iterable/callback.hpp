#ifndef _SABRE_OBJECT_ITERABLE_HPP
#define _SABRE_OBJECT_ITERABLE_HPP

/// Sabre Includes
#include "sabre/object/mixin.hpp"

namespace Sabre::Iterable {

/// @brief Iteration Callback.
template <std::derived_from<Value::Any> T>
using Callback = $::Unique::Functor<Value::Any(Runtime::Isolate *, const T &, size_t) const>;

} // namespace Sabre::Iterable

#endif
