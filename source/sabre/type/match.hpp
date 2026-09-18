#ifndef _SABRE_TYPE_MATCH_HPP
#define _SABRE_TYPE_MATCH_HPP

/// Sabre Includes
#include "sabre/type/deduction.hpp"

namespace Sabre::Type {

/// @brief Match Statement Output.
struct Match {
  //  PROPERTIES  //

  /// @brief Denotes if a fallback was seen.
  bool fallback = false;

  /// @brief Outgoing match result (holds value type).
  Deduction result = New::any();
};

} // namespace Sabre::Type

#endif
