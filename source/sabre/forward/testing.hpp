#ifndef _SABRE_FORWARD_TESTING_HPP
#define _SABRE_FORWARD_TESTING_HPP

/// Vendor Includes
#include <xtest/xtest.hpp>

/// Forward Declarations
$_FWD(Sabre::Testing, class Entry)
$_FWD(Sabre::Testing, class Service)
$_FWD(Sabre::Testing, struct Options)
$_FWD(Sabre::Testing, struct Storage)

/// Forward Definitions
$_FWD(Sabre::Testing, using Case = XT::Handle::Case)
$_FWD(Sabre::Testing, using Bench = XT::Bench::Mark)
$_FWD(Sabre::Testing, using Group = XT::Handle::Group)
$_FWD(Sabre::Testing, using Callback = XT::Handle::Callback)
$_FWD(Sabre::Testing, using Runner = XT::Session::Runner)
$_FWD(Sabre::Testing, using Handle = $::Unique::Pointer<XT::Handle::Base>)

#endif
