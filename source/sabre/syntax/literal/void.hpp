#ifndef _SABRE_LITERAL_VOID_HPP
#define _SABRE_LITERAL_VOID_HPP

/// Sabre Includes
#include "sabre/syntax/node.hpp"

namespace Sabre::Syntax {

/// @brief Void Literal Node.
struct Void : public Mixin<Void, Expression> {
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  explicit Void() = default;
};

} // namespace Sabre::Syntax

#endif
