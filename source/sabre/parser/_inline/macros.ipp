#ifndef _SABRE_PARSER_MACROS_IPP
#define _SABRE_PARSER_MACROS_IPP

/// Sabre Includes
#include "sabre/parser/dispatch.hpp"

//  MACROS  //

#define SABRE_MM_PARSE_FWD(N, R, T, ...)                                                      \
  template <> Sabre::Syntax::R *Sabre::Parser::Dispatch::m_##N<Sabre::Syntax::T>(__VA_ARGS__)

#define SABRE_MM_PARSE_DECL(T, P, ...) SABRE_MM_PARSE_FWD(declaration, T, T, Stream *P)
#define SABRE_MM_PARSE_STMT(T, P, ...) SABRE_MM_PARSE_FWD(statement, T, T, Stream *P)
#define SABRE_MM_PARSE_HINT(T, P, ...) SABRE_MM_PARSE_FWD(annotation, T, T, Stream *P)
#define SABRE_MM_PARSE_PREFIX(T, P, A, ...) SABRE_MM_PARSE_FWD(prefix, Expression, T, Stream *P, bool A)
#define SABRE_MM_PARSE_INFIX(T, P, E, A, ...)                                        \
  SABRE_MM_PARSE_FWD(infix, Expression, T, Stream *P, Syntax::Expression *E, bool A)

#endif
