#ifndef _XTDLIB_RANDOM_STANDARD_HPP
#define _XTDLIB_RANDOM_STANDARD_HPP

/// Library Includes
#include "xtdlib/random/interface.hpp"

namespace $::Random::Device {

/// @brief Standard RNG Device.
struct Standard : public std::random_device {
  //  TYPEDEFS  //

  /// @brief Expose the baseline output typing.
  using Output = result_type;

  //  CONSTRUCTORS  //

  /// @brief Inherit the baseline constructors.
  using std::random_device::random_device;

  //  PUBLIC METHODS  //

  /// @brief Gets the next random number.
  inline constexpr Output next() { return (*this)(); }
};

// ensure the simple device is a valid instance
static_assert(Interface<Standard>, "Invalid Random Device Interface");

} // namespace $::Random::Device

#endif
