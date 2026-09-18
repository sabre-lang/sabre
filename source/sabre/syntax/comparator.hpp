#ifndef _SABRE_SYNTAX_COMPARATOR_HPP
#define _SABRE_SYNTAX_COMPARATOR_HPP

/// Sabre Includes
#include "sabre/syntax/node.hpp"

namespace Sabre::Syntax {

/// @brief Ranges Heap Comparator.
struct Comparator {
  //  CONSTRUCTORS  //

  /// @brief Constructs a comparator function.
  explicit Comparator() = default;

  //  OPERATOR METHODS  //

  /// @brief Compares two nodes for their range position.
  inline constexpr bool operator()(const Syntax::Node *left, const Syntax::Node *right) const {
    return left->trivia()->range() < right->trivia()->range();
  }

  //  PUBLIC METHODS  //

  /// @brief Checks for nodes that have a valid range.
  inline constexpr bool validate(const Syntax::Node *node) const { return node->trivia()->range() != XLSP::Range(); }
};

} // namespace Sabre::Syntax

#endif
