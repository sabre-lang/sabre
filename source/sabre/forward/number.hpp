#ifndef _SABRE_FORWARD_NUMBER_HPP
#define _SABRE_FORWARD_NUMBER_HPP

/// Vendor Includes
#include <xtdlib/xtdlib.hpp>

/// Forward Declarations
$_FWD(Sabre::Number, struct Bignum)
$_FWD(Sabre::Number, struct Tagged)

/// Forward Definitions
$_FWD(Sabre::Number, using Floating = $_ARCH_TYPED(double, float))
$_FWD(Sabre::Number, using Integral = $_ARCH_TYPED(int64_t, int32_t))

#endif
