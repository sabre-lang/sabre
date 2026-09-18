#ifndef _SABRE_GARBAGE_MODE_HPP
#define _SABRE_GARBAGE_MODE_HPP

/// Sabre Includes
#include "sabre/forward/garbage.hpp"

namespace Sabre::Garbage {

/// @brief Potential Collection Modes
enum class Mode : bool { MINOR, MAJOR };

} // namespace Sabre::Garbage

#endif
