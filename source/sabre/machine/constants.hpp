#ifndef _SABRE_MACHINE_CONSTANTS_HPP
#define _SABRE_MACHINE_CONSTANTS_HPP

/// Sabre Includes
#include "sabre/forward/machine.hpp"
#include "sabre/value/void.hpp"

namespace Sabre::Machine::Constants {

static inline constexpr Value::Any Void = Value::Void();
static inline constexpr Value::Any Fail = Value::Failure();

} // namespace Sabre::Machine::Constants

#endif
