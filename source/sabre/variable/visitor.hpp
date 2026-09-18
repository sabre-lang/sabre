#ifndef _SABRE_VARIABLE_VISITOR_HPP
#define _SABRE_VARIABLE_VISITOR_HPP

/// Sabre Includes
#include "sabre/syntax/visitor.hpp"
#include "sabre/variable/analyzer.hpp"

/// Forward Declarations
$_FWD(Sabre::Variable, using Visitor = Syntax::Visitor<void, Analyzer *>)

//  MACROS  //

/// @brief Allows variable capturing of nodes.
#define SABRE_MM_CAPTURE_NODE(T, N, A, ...)                                                                      \
  template <> void Sabre::Variable::Visitor::Acceptor::accept<Sabre::Syntax::T>(const Syntax::T *N, Analyzer *A)

//  NAMESPACES  //

namespace Sabre::Syntax {

/// @brief Visitor Specialization.
template <> struct Visitor<void, ::Sabre::Variable::Analyzer *>::Acceptor {
  //  TYPEDEFS  //

  /// @brief Aliased analyzer instance.
  using Analyzer = ::Sabre::Variable::Analyzer;

  //  PUBLIC METHODS  //

  /// @brief Allows visiting syntax nodes for analysis.
  template <std::derived_from<Node> T> static void accept(const T *node, Analyzer *analyzer);
};

} // namespace Sabre::Syntax

//  SPECIALIZATIONS  //

#define X(T, ...) SABRE_MM_CAPTURE_NODE(T, , );
SABRE_XX_SYNTAX_NODES(X)
#undef X

#endif
