#ifndef _SABRE_HEAP_LIMITS_HPP
#define _SABRE_HEAP_LIMITS_HPP

/// Sabre Includes
#include "sabre/forward/heap.hpp"

namespace Sabre::Heap::Limits {
//  PROPERTIES  //

static constexpr size_t REGIONS_INITIAL = 32;                    // Minimum allowable regions.
static constexpr size_t ADDRESS_SPACE = 4 * $::Memory::Size::GB; // Maximum heap size available.

/// @brief Maximum region size available (this is fixed on Windows for now).
static constexpr size_t REGIONS_SIZE = ($_PLATFORM_WINDOWS ? 64 : 256) * $::Memory::Size::KB;

static constexpr size_t REGIONS_MASK = ~(REGIONS_SIZE - 1);           // Mask for forwarding objects.
static constexpr size_t REGIONS_TOTAL = ADDRESS_SPACE / REGIONS_SIZE; // Total regions available.

} // namespace Sabre::Heap::Limits

#endif
