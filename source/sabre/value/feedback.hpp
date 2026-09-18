#ifndef _SABRE_VALUE_FEEDBACK_HPP
#define _SABRE_VALUE_FEEDBACK_HPP

/// Sabre Includes
#include "sabre/forward/value.hpp"

namespace Sabre::Value {

/// @brief Associated Feedback Values.
enum class Feedback : uint8_t {
  ALL_OKAY = 0b0000,
  ITER_SENTINEL = 0b0001,

  ERROR_THROWN = 0b1000,
  ASYNC_TIMEOUT = 0b1001,
  FIELD_MISSING = 0b1010,
  FIELD_IMMUTABLE = 0b1011,
};

} // namespace Sabre::Value

#endif
