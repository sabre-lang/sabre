#ifndef _SABRE_VARIABLE_UPVALUES_HPP
#define _SABRE_VARIABLE_UPVALUES_HPP

/// Sabre Includes
#include "sabre/bytecode/allocator.hpp"
#include "sabre/bytecode/label.hpp"
#include "sabre/forward/variable.hpp"

namespace Sabre::Variable {

/// @brief Variable Lookup Result.
struct Upvalue {
  Bytecode::Index depth; // Frame depth away.
  Register::Slot slot;   // Index of upvalue.
};

} // namespace Sabre::Variable

#endif
