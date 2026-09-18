#ifndef _SABRE_HEAP_OPTIONS_HPP
#define _SABRE_HEAP_OPTIONS_HPP

/// Sabre Includes
#include "sabre/heap/limits.hpp"
#include "sabre/value/pointer.hpp"

namespace Sabre::Heap {

/// @brief Heap Address Typing.
using Address = Pointer::Underlying;

/// @brief Heap Options.
struct Options {
  //  PROPERTIES  //

  size_t garbage_attempts = 4;   // GC attempts allowed.
  size_t garbage_survivors = 15; // Senior survivor count.

  float growth_factor = 1.5f; // Default heap growth factor.
  float shrink_factor = 0.5f; // Default heap shrink factor.

  //  CONSTRUCTORS  //

  /// @brief Constructs a defaulted set of options.
  constexpr Options() = default;
};

} // namespace Sabre::Heap

#endif
