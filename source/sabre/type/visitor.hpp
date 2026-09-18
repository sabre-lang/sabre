#ifndef _SABRE_TYPE_VISITOR_HPP
#define _SABRE_TYPE_VISITOR_HPP

/// Sabre Includes
#include "sabre/syntax/visitor.hpp"
#include "sabre/type/analyzer.hpp"

/// Forward Declarations
$_FWD(Sabre::Type, using Visitor = Syntax::Visitor<Deduction, Analyzer *>)

//  MACROS  //

/// @brief Allows checking of nodes.
#define SABRE_MM_CHECK_NODE(T, N, A, ...)                                          \
  template <>                                                                      \
  Sabre::Type::Deduction Sabre::Type::Visitor::Acceptor::accept<Sabre::Syntax::T>( \
      const Syntax::T *N, Type::Analyzer *A                                        \
  )

//  NAMESPACES  //

namespace Sabre::Syntax {

/// @brief Visitor Specialization.
template <> struct Visitor<Type::Deduction, Type::Analyzer *>::Acceptor {
  //  PUBLIC METHODS  //

  /// @brief Allows visiting syntax nodes for analysis.
  template <std::derived_from<Node> T> static Type::Deduction accept(const T *node, Type::Analyzer *analyzer);
};

} // namespace Sabre::Syntax

//  SPECIALIZATIONS  //

#define X(T, ...) SABRE_MM_CHECK_NODE(T, , );
SABRE_XX_SYNTAX_NODES(X)
#undef X

#endif
