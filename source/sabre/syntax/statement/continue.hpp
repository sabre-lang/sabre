#ifndef _SABRE_STATEMENT_CONTINUE_HPP
#define _SABRE_STATEMENT_CONTINUE_HPP

/// Sabre Includes
#include "sabre/syntax/node.hpp"

namespace Sabre::Syntax {

/// @brief Continue Statement.
struct Continue : public Mixin<Continue, Statement> {
  //  CONSTRUCTORS  //

  /// @brief Constructs a control statement.
  explicit Continue() = default;
};

} // namespace Sabre::Syntax

#endif
