#ifndef _SABRE_SHAPE_LIMITS_HPP
#define _SABRE_SHAPE_LIMITS_HPP

/// Sabre Includes
#include "sabre/forward/shape.hpp"

/// Value Includes
#include "sabre/value/_inline/macros.ipp"

namespace Sabre::Shape::Limits {

//  PROPERTIES  //

static constexpr Underlying MAXIMUM = UINT32_MAX;
static constexpr Underlying THRESHOLD =
    SABRE_XX_VALUES_IMMEDIATE($_XX_ENUM_COUNT) + SABRE_XX_VALUES_OBJECT($_XX_ENUM_COUNT) 1;

} // namespace Sabre::Shape::Limits

#endif
