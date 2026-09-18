#ifndef _SABRE_FLOW_EFFECT_HPP
#define _SABRE_FLOW_EFFECT_HPP

/// Sabre Includes
#include "sabre/forward/flow.hpp"

namespace Sabre::Flow {

/// @brief Potential Side-Effects.
enum class Effect : uint8_t { NONE, BREAKS, LOOPS };

} // namespace Sabre::Flow

#endif
