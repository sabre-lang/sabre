#ifndef _SABRE_SYNTAX_VISITOR_HPP
#define _SABRE_SYNTAX_VISITOR_HPP

/// Sabre Includes
#include "sabre/syntax/tree.hpp"

/// Inline Includes
#include "sabre/syntax/_inline/annotation.ipp"
#include "sabre/syntax/_inline/declaration.ipp"
#include "sabre/syntax/_inline/expression.ipp"
#include "sabre/syntax/_inline/statement.ipp"

//  X-MACROS  //

/// @brief All available syntax nodes.
#define SABRE_XX_SYNTAX_NODES(X) \
  SABRE_XX_EXPRESSION_NODES(X)   \
  SABRE_XX_STATEMENT_NODES(X)    \
  SABRE_XX_DECLARATION_NODES(X)  \
  SABRE_XX_ANNOTATION_NODES(X)   \
  SABRE_XX_ANONYMOUS_NODES(X)

//  NAMESPACES  //

namespace Sabre::Syntax {

/// @brief Baseline Syntax Visitor.
template <class R, class... As> struct Visitor {
  //  TYPEDEFS  //

  /// @brief Acceptor instance.
  struct Acceptor;

  //  CONSTRUCTORS  //

  /// @brief Constructs a visitor instance.
  constexpr Visitor() = default;

  //  PUBLIC METHODS  //

  /**
   * @brief Handles visiting nodes.
   * @param node                      Node to visit.
   * @param args                      Arguments to forward.
   */
  $_INLINE_PERF static constexpr R visit(const Node *node, As &&...args) {
#define X(N, ...)                                                                             \
  case $::RTTI::Hash<N>(): return Acceptor::accept(node->as<N>(), std::forward<As>(args)...);
    switch (node->trivia()->hash()) {
      SABRE_XX_SYNTAX_NODES(X) // should be a suitable node to handle
    default: $_ABORT("Unknown AST Node '{0}'", node->trivia()->name());
    }
#undef X
  }
};

} // namespace Sabre::Syntax

#endif
