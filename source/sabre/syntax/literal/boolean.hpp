#ifndef _SABRE_LITERAL_BOOLEAN_HPP
#define _SABRE_LITERAL_BOOLEAN_HPP

/// Sabre Includes
#include "sabre/syntax/node.hpp"

namespace Sabre::Syntax {

/// @brief True Literal Node.
struct True : public Mixin<True, Expression> {
  //  CONSTRUCTORS  //

  /// @brief Constructs a literal "True" value.
  explicit True() = default;
};

/// @brief False Literal Node.
struct False : public Mixin<False, Expression> {
  //  CONSTRUCTORS  //

  /// @brief Constructs a literal "False" value.
  explicit False() = default;
};

} // namespace Sabre::Syntax

#endif
