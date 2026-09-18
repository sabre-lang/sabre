#ifndef _SABRE_FUNCTION_POLICY_HPP
#define _SABRE_FUNCTION_POLICY_HPP

/// Sabre Includes
#include "sabre/forward/function.hpp"

//  X-MACROS  //

/// @brief All available function policies.
#define SABRE_XX_FUNCTION_POLICIES(X)                \
  X(CALL, "call")   /** Normal call invocation. */   \
  X(ASYNC, "async") /** Default async invocation. */ \
  X(DEFER, "defer") /** Defers scoped execution. */  \
  X(SPAWN, "spawn") /** Custom async invocation. */

//  NAMESPACES  //

namespace Sabre::Function {

/// @brief Available Function Policies.
$_XX_ENUM_CLASS(Policy, uint8_t, SABRE_XX_FUNCTION_POLICIES);

} // namespace Sabre::Function

#endif
