#ifndef _SABRE_FORWARD_RELINT_HPP
#define _SABRE_FORWARD_RELINT_HPP

/// Vendor Includes
#include <xtdlib/xtdlib.hpp>

/// Sabre Includes
#include "sabre/forward/diagnostic.hpp"

/// Forward Declarations
$_FWD(Sabre::Relint, struct Rule)
$_FWD(Sabre::Relint, class Hooks)
$_FWD(Sabre::Relint, class Mirror)
$_FWD(Sabre::Relint, struct Options)
$_FWD(Sabre::Relint, class Exports)
$_FWD(Sabre::Relint, class Scope)
$_FWD(Sabre::Relint, class Context)
$_FWD(Sabre::Relint, class Comments)
$_FWD(Sabre::Relint, struct Analyzer)
$_FWD(Sabre::Relint, struct Definition)

/// Forward Definitions
$_FWD(Sabre::Relint, using Severity = Diagnostic::Severity)
$_FWD(Sabre::Relint, using Factory = $::Unique::Functor<void(Hooks *, const Rule *) const>)

#endif
