#ifndef _SABRE_RELINT_RULES_IPP
#define _SABRE_RELINT_RULES_IPP

/// Sabre Includes
#include "sabre/relint/context.hpp"
#include "sabre/relint/service.hpp"

//  X-MACROS  //

#define SABRE_XX_RELINT_RULES(X)                    \
  X(NO_EMPTY_BLOCKS, "no-empty-blocks")             \
  X(NO_UNSAFE_ACCESSORS, "no-unsafe-accessors")     \
  X(NO_UNSAFE_ASSIGNMENTS, "no-unsafe-assignments") \
  X(NO_UNSAFE_CALLS, "no-unsafe-calls")             \
  X(NO_UNSAFE_RETURNS, "no-unsafe-returns")

//  NAMESPACES  //

namespace Sabre::Relint::Label {

#define X(N, L, ...) static constexpr $::String::Literal N = L;
SABRE_XX_RELINT_RULES(X)
#undef X

} // namespace Sabre::Relint::Label

#endif
