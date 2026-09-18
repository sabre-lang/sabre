#ifndef _SABRE_OPERATOR_ATTRIBUTE_HPP
#define _SABRE_OPERATOR_ATTRIBUTE_HPP

/// Sabre Includes
#include "sabre/operator/inspect.hpp"

namespace Sabre::Operator::Attribute {

#define X(K, N, ...) static constexpr auto K = Inspect::symbol(Kind::K);
SABRE_XX_OPERATORS_CUSTOM(X)
X(CALL, "invoke")
#undef X

} // namespace Sabre::Operator::Attribute

#endif
