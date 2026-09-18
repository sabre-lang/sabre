#ifndef _SABRE_ANNOTATION_GENERIC_HPP
#define _SABRE_ANNOTATION_GENERIC_HPP

/// Syntax Includes
#include "sabre/syntax/annotation/placeholder.hpp"

namespace Sabre::Syntax {

/// @brief Generic Parameters Container.
using Template = std::vector<Placeholder *>;

/// @brief Generic Arguments Container.
using Specialization = std::vector<Annotation *>;

} // namespace Sabre::Syntax

#endif
