#ifndef _SABRE_SYNTAX_ANNOTATION_IPP
#define _SABRE_SYNTAX_ANNOTATION_IPP

/// Sabre Includes
#include "sabre/parser/dispatch.hpp"

/// Parser Includes
#include "sabre/parser/_inline/macros.ipp"

/// Annotation Includes
#include "sabre/syntax/annotation/constructor.hpp"
#include "sabre/syntax/annotation/interface.hpp"
#include "sabre/syntax/annotation/parameters.hpp"
#include "sabre/syntax/annotation/placeholder.hpp"
#include "sabre/syntax/annotation/qualifier.hpp"
#include "sabre/syntax/annotation/signature.hpp"

//  MACROS  //

/// @brief All available type-annotation nodes.
#define SABRE_XX_ANNOTATION_NODES(X) \
  X(Qualifier)                       \
  X(Placeholder)                     \
  X(Parameters)                      \
  X(Constructor)                     \
  X(Signature)                       \
  X(Interface)

//  SPECIALIZATIONS  //

#define X(N, ...) SABRE_MM_PARSE_HINT(N, );
SABRE_XX_ANNOTATION_NODES(X)
#undef X

SABRE_MM_PARSE_DECL(Parameters, );
SABRE_MM_PARSE_DECL(Constructor, );
SABRE_MM_PARSE_DECL(Signature, );

#endif
