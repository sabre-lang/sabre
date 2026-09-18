#ifndef _SABRE_BUILTINS_ADAPTER_HPP
#define _SABRE_BUILTINS_ADAPTER_HPP

/// Sabre Includes
#include "sabre/forward/builtins.hpp"

namespace Sabre::Builtins {

/// @brief Associated Adapter Enumeration.
enum class Adapter : uint8_t {
  GLOBALS,   // Global value registration.
  TYPEDEFS,  // Type definition registration.
  PROTOTYPE, // Defines a class-prototype.

  FIELDS,    // Attributes callbacks resolution.
  OPERATORS, // Operator callback resolution.
};

} // namespace Sabre::Builtins

#endif
