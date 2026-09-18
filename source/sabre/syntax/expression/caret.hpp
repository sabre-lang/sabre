#ifndef _SABRE_EXPRESSION_CARET_HPP
#define _SABRE_EXPRESSION_CARET_HPP

/// Sabre Includes
#include "sabre/syntax/node.hpp"

namespace Sabre::Syntax {

/// @brief Caret Expression Node.
struct Caret : public Mixin<Caret, Expression> {
  //  CONSTRUCTORS  //

  /// @brief Constructs a caret expression.
  explicit Caret() = default;
};

} // namespace Sabre::Syntax

#endif
