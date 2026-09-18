#ifndef _SABRE_SYNTAX_EXPRESSION_IPP
#define _SABRE_SYNTAX_EXPRESSION_IPP

/// Sabre Includes
#include "sabre/parser/dispatch.hpp"

/// Parser Includes
#include "sabre/parser/_inline/macros.ipp"

/// Literal Includes
#include "sabre/syntax/literal/boolean.hpp"
#include "sabre/syntax/literal/identifier.hpp"
#include "sabre/syntax/literal/lambda.hpp"
#include "sabre/syntax/literal/numeric.hpp"
#include "sabre/syntax/literal/record.hpp"
#include "sabre/syntax/literal/text.hpp"
#include "sabre/syntax/literal/tuple.hpp"
#include "sabre/syntax/literal/void.hpp"

/// Expression Includes
#include "sabre/syntax/expression/accessor.hpp"
#include "sabre/syntax/expression/assign.hpp"
#include "sabre/syntax/expression/binary.hpp"
#include "sabre/syntax/expression/call.hpp"
#include "sabre/syntax/expression/caret.hpp"
#include "sabre/syntax/expression/cast.hpp"
#include "sabre/syntax/expression/execute.hpp"
#include "sabre/syntax/expression/group.hpp"
#include "sabre/syntax/expression/infer.hpp"
#include "sabre/syntax/expression/panic.hpp"
#include "sabre/syntax/expression/ternary.hpp"
#include "sabre/syntax/expression/typed.hpp"
#include "sabre/syntax/expression/unary.hpp"

//  MACROS  //

/// @brief All available literal-expression nodes.
#define SABRE_XX_LITERAL_NODES(X) \
  X(Self)                         \
  X(Text)                         \
  X(Void)                         \
  X(True)                         \
  X(False)                        \
  X(Tuple)                        \
  X(Lambda)                       \
  X(Record)                       \
  X(Numeric)                      \
  X(Identifier)

/// @brief All available prefix-expression nodes.
#define SABRE_XX_PREFIX_NODES(X) \
  X(Unary)                       \
  X(Caret)                       \
  X(Group)                       \
  X(Panic)

/// @brief All available infix-expression nodes.
#define SABRE_XX_INFIX_NODES(X) \
  X(Binary)                     \
  X(Ternary)                    \
  X(Call)                       \
  X(Cast)                       \
  X(Infer)                      \
  X(Typed)                      \
  X(Assign)                     \
  X(Execute)                    \
  X(Accessor)

/// @brief All available expression nodes.
#define SABRE_XX_EXPRESSION_NODES(X) \
  SABRE_XX_LITERAL_NODES(X)          \
  SABRE_XX_PREFIX_NODES(X)           \
  SABRE_XX_INFIX_NODES(X)

//  SPECIALIZATIONS  //

#define X(N, ...) SABRE_MM_PARSE_PREFIX(N, , );
SABRE_XX_LITERAL_NODES(X)
SABRE_XX_PREFIX_NODES(X)
#undef X

#define X(N, ...) SABRE_MM_PARSE_INFIX(N, , , );
SABRE_XX_INFIX_NODES(X)
#undef X

#endif
