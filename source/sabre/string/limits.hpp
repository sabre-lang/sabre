#ifndef _SABRE_STRING_LIMITS_HPP
#define _SABRE_STRING_LIMITS_HPP

/// Sabre Includes
#include "sabre/value/common.hpp"

namespace Sabre::String::Limits {

/// @brief How big a small tagged string is.
static constexpr size_t SMALL = Pointer::Alignment::VALUE - 1;

} // namespace Sabre::String::Limits

#endif
