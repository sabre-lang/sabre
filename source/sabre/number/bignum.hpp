#ifndef _SABRE_NUMBER_BIGNUM_HPP
#define _SABRE_NUMBER_BIGNUM_HPP

/// Sabre Includes
#include "sabre/object/mixin.hpp"

namespace Sabre {

/// TODO: Implement a "Bignum" object.

/// @brief Large Number Attributes.
template <> struct Object::Wrapper<Number::Bignum> {};

/// @brief Arbitrary Precision Number.
struct Number::Bignum : public Object::Mixin<Bignum> {};

} // namespace Sabre

#endif
