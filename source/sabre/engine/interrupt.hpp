#ifndef _SABRE_ENGINE_INTERRUPT_HPP
#define _SABRE_ENGINE_INTERRUPT_HPP

/// Sabre Includes
#include "sabre/forward/engine.hpp"

namespace Sabre::Engine {

/// @brief Available Interrupt Modes.
enum class Interrupt : uint8_t {
  BAILOUT, // Forces a bailout.
  DISPOSE, // Flags disposables.
};

} // namespace Sabre::Engine

#endif
