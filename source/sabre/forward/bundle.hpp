#ifndef _SABRE_FORWARD_BUNDLE_HPP
#define _SABRE_FORWARD_BUNDLE_HPP

/// Vendor Includes
#include <xjct/xjct.hpp>

/// Sabre Includes
#include "sabre/forward/runtime.hpp"

/// Forward Declarations
$_FWD(Sabre::Bundle, class Archive)
$_FWD(Sabre::Bundle, struct Options)
$_FWD(Sabre::Bundle, class Service)
$_FWD(Sabre::Bundle, class Worker)
$_FWD(Sabre::Bundle, struct Filesystem)

/// Forward Definitions
$_FWD(Sabre::Bundle, using Provider = XJCT::Archive::Provider)
$_FWD(Sabre::Bundle, using Executable = XJCT::Archive::Binary)

#endif
