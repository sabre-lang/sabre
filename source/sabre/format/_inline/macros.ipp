#ifndef _SABRE_FORMAT_MACROS_IPP
#define _SABRE_FORMAT_MACROS_IPP

/// Sabre Includes
#include "sabre/format/dispatch.hpp"
#include "sabre/syntax/visitor.hpp"

//  MACROS  //

#define SABRE_MM_FORMAT_FWD(N, T, ...)                                                            \
  template <> Sabre::Format::Piece *Sabre::Format::Dispatch::m_##N<Sabre::Syntax::T>(__VA_ARGS__)

#define SABRE_MM_FORMAT_DECL(T, R, ...) SABRE_MM_FORMAT_FWD(declaration, T, Reader *R)
#define SABRE_MM_FORMAT_STMT(T, R, G, ...) SABRE_MM_FORMAT_FWD(statement, T, Reader *R, Piece::Group *G)
#define SABRE_MM_FORMAT_HINT(T, R, ...) SABRE_MM_FORMAT_FWD(annotation, T, Reader *R)
#define SABRE_MM_FORMAT_PREFIX(T, R, ...) SABRE_MM_FORMAT_FWD(prefix, T, Reader *R, bool)
#define SABRE_MM_FORMAT_INFIX(T, R, E, ...) SABRE_MM_FORMAT_FWD(infix, T, Reader *R, Piece *E, bool)

//  SPECIALIZATIONS  //

SABRE_MM_FORMAT_HINT(Node, , );
SABRE_MM_FORMAT_DECL(Node, , );
SABRE_MM_FORMAT_STMT(Node, , , );

#define X(N, ...) SABRE_MM_FORMAT_HINT(N, , );
SABRE_XX_ANNOTATION_NODES(X)
#undef X

#define X(N, ...) SABRE_MM_FORMAT_DECL(N, , );
X(Parameters);
X(Constructor);
X(Signature);

X(Declaration);
SABRE_XX_ANONYMOUS_NODES(X)
SABRE_XX_DECLARATION_NODES(X)
#undef X

#define X(N, ...) SABRE_MM_FORMAT_STMT(N, , , );
SABRE_XX_STATEMENT_NODES(X)
#undef X

#endif
