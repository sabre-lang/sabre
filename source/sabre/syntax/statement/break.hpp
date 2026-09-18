#ifndef _SABRE_STATEMENT_BREAK_HPP
#define _SABRE_STATEMENT_BREAK_HPP

/// Sabre Includes
#include "sabre/syntax/node.hpp"

namespace Sabre::Syntax {

/// @brief Break Statement.
struct Break : public Mixin<Break, Statement> {
  //  CONSTRUCTORS  //

  /// @brief Constructs a control statement.
  explicit Break() = default;
};

} // namespace Sabre::Syntax

#endif
