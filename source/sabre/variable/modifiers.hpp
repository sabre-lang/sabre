#ifndef _SABRE_VARIABLE_MODIFIERS_HPP
#define _SABRE_VARIABLE_MODIFIERS_HPP

/// Sabre Includes
#include "sabre/forward/variable.hpp"

namespace Sabre::Variable {

/// @brief Variable Flags Available.
enum class Flag : uint16_t {
  MUTABLE,    // Denotes readonly/mutability.
  OPTIONAL,   // Whether or not a typing is optional.
  DISPOSABLE, // Denotes if variable is disposable.

  VIRTUAL,  // Encapsulates virtual/abstract variables.
  OVERRIDE, // Encapsulates field/virtual overrides.

  PUBLIC,    // Publicly available fields.
  PRIVATE,   // Privately available fields.
  PROTECTED, // Protected object fields.

  EXPORT, // Denotes if a variable has been exported.
  OWNED,  // Denotes if a variable is used/owned at all.
  NOTICE, // Denotes a deprecation notice as attached.
};

/// @brief Explicit Modifier Flags.
using Modifiers = $::Enum::Flags<Flag>;

} // namespace Sabre::Variable

#endif
