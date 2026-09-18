#ifndef _SABRE_FORWARD_DYLIB_HPP
#define _SABRE_FORWARD_DYLIB_HPP

/// Sabre Includes
#include "sabre/object/instance.hpp"

/// Forward Declarations
$_FWD(Sabre::Dylib, class Addon)
$_FWD(Sabre::Dylib, class Registry)

/// Forward Definitions
$_FWD(Sabre::Dylib, using Exports = Object::Instance)
$_FWD(Sabre::Dylib, using Loader = $::Unique::Functor<void(Runtime::Isolate *, Exports &)>)

#endif
