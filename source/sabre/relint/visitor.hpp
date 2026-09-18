#ifndef _SABRE_RELINT_VISITOR_HPP
#define _SABRE_RELINT_VISITOR_HPP

/// Sabre Includes
#include "sabre/relint/analyzer.hpp"
#include "sabre/syntax/visitor.hpp"

/// Forward Declarations
$_FWD(Sabre::Relint, using Visitor = Syntax::Visitor<void, Analyzer *>)

//  MACROS  //

#define SABRE_MM_LINT_NODE(T, N, A, ...)                                                                               \
  template <> void Sabre::Relint::Visitor::Acceptor::accept<Sabre::Syntax::T>(const Syntax::T *N, Relint::Analyzer *A)

//  NAMESPACES  //

namespace Sabre::Syntax {

/// @brief Visitor Specialization.
template <> struct Visitor<void, Relint::Analyzer *>::Acceptor {
  //  PUBLIC METHODS  //

  template <std::derived_from<Node> T> static void accept(const T *node, Relint::Analyzer *analyzer);
};

} // namespace Sabre::Syntax

//  SPECIALIZATIONS  //

#define X(T, ...) SABRE_MM_LINT_NODE(T, , );
SABRE_XX_SYNTAX_NODES(X)
#undef X

#endif
