#ifndef _SABRE_FORWARD_GLOBALS_HPP
#define _SABRE_FORWARD_GLOBALS_HPP

/// Sabre Includes
#include "sabre/forward/runtime.hpp"
#include "sabre/forward/value.hpp"

/// Forward Declarations
$_FWD(Sabre::Globals, class Each)
$_FWD(Sabre::Globals, class Roots)
$_FWD(Sabre::Globals, class Service)

/// Forward Definitions
$_FWD(Sabre::Globals, using Visit = $::Shared::Functor<void(Value::Any &)>)
$_FWD(Sabre::Globals, using Factory = $::Unique::Functor<Value::Any(Runtime::Isolate *)>)

#endif
