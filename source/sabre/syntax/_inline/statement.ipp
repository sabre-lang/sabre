#ifndef _SABRE_SYNTAX_STATEMENT_IPP
#define _SABRE_SYNTAX_STATEMENT_IPP

/// Sabre Includes
#include "sabre/parser/dispatch.hpp"

/// Parser Includes
#include "sabre/parser/_inline/macros.ipp"

/// Statement Includes
#include "sabre/syntax/statement/block.hpp"
#include "sabre/syntax/statement/break.hpp"
#include "sabre/syntax/statement/conditional.hpp"
#include "sabre/syntax/statement/continue.hpp"
#include "sabre/syntax/statement/import.hpp"
#include "sabre/syntax/statement/loop.hpp"
#include "sabre/syntax/statement/match.hpp"
#include "sabre/syntax/statement/return.hpp"

//  MACROS  //

/// @brief All available statement nodes.
#define SABRE_XX_STATEMENT_NODES(X) \
  X(Break)                          \
  X(Return)                         \
  X(Continue)                       \
                                    \
  X(For)                            \
  X(Loop)                           \
  X(Block)                          \
  X(Match)                          \
  X(Query)                          \
  X(Conditional)                    \
                                    \
  X(Import)

//  SPECIALIZATIONS  //

#define X(N, ...) SABRE_MM_PARSE_STMT(N, );
SABRE_XX_STATEMENT_NODES(X)
#undef X

#endif
