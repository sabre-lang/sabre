#ifndef _XTEST_REPORTER_PROXY_HPP
#define _XTEST_REPORTER_PROXY_HPP

/// Testing Includes
#include "xtest/forward/reporter.hpp"

//  X-MACROS  //

/// @brief Available Reporter Types.
#define XTEST_XX_REPORTER_LABELS(X) \
  X(TEXT, "text")                   \
  X(TAP, "tap")                     \
  X(XML, "xml")                     \
  X(JSON, "json")                   \
  X(JUNIT, "junit")

//  NAMESPACES  //

namespace XT::Reporter {

#define X(T, N, ...) $_FWD(Label, static inline constexpr $::String::Literal T = N)
XTEST_XX_REPORTER_LABELS(X)
X(DEFAULT, TEXT)
#undef X

/// @brief A proxy reporter for defining different types.
template <$::String::Literal T> class Proxy;

} // namespace XT::Reporter

#endif
