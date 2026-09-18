#ifndef _SABRE_SYNTAX_DECLARATION_IPP
#define _SABRE_SYNTAX_DECLARATION_IPP

/// Sabre Includes
#include "sabre/parser/dispatch.hpp"

/// Parser Includes
#include "sabre/parser/_inline/macros.ipp"

/// Attribute Includes
#include "sabre/syntax/attribute/deprecated.hpp"
#include "sabre/syntax/attribute/internal.hpp"
#include "sabre/syntax/attribute/overload.hpp"
#include "sabre/syntax/attribute/override.hpp"

/// Declaration Includes
#include "sabre/syntax/declaration/alias.hpp"
#include "sabre/syntax/declaration/class.hpp"
#include "sabre/syntax/declaration/enum.hpp"
#include "sabre/syntax/declaration/namespace.hpp"
#include "sabre/syntax/declaration/preamble.hpp"
#include "sabre/syntax/declaration/variable.hpp"

//  MACROS  //

/// @brief All available anonymous nodes.
#define SABRE_XX_ANONYMOUS_NODES(X) \
  X(Tree)                           \
  X(Header)                         \
  X(Variant)                        \
  X(Decorator)                      \
  X(Fallback)                       \
                                    \
  X(Internal)                       \
  X(Overload)                       \
  X(Override)                       \
  X(Deprecated)

/// @brief All available declaration nodes.
#define SABRE_XX_DECLARATION_NODES(X) \
  X(Enum)                             \
  X(Alias)                            \
  X(Class)                            \
  X(Variable)                         \
  X(Namespace)

//  SPECIALIZATIONS  //

#define X(N, ...) SABRE_MM_PARSE_DECL(N, );
X(Declaration)
SABRE_XX_ANONYMOUS_NODES(X)
SABRE_XX_DECLARATION_NODES(X)
#undef X

#endif
