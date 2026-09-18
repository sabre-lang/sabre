#ifndef _SABRE_BUILTINS_INLINE_IPP
#define _SABRE_BUILTINS_INLINE_IPP

/// Sabre Includes
#include "sabre/builtins/storage.hpp"

/// Builtins Includes
#include "sabre/builtins/boolean/traits.hpp"
#include "sabre/builtins/class/traits.hpp"
#include "sabre/builtins/debug/traits.hpp"
#include "sabre/builtins/enum/traits.hpp"
#include "sabre/builtins/exception/traits.hpp"
#include "sabre/builtins/function/traits.hpp"
#include "sabre/builtins/future/traits.hpp"
#include "sabre/builtins/iterator/traits.hpp"
#include "sabre/builtins/list/traits.hpp"
#include "sabre/builtins/number/traits.hpp"
#include "sabre/builtins/object/traits.hpp"
#include "sabre/builtins/result/traits.hpp"
#include "sabre/builtins/string/traits.hpp"
#include "sabre/builtins/symbol/traits.hpp"
#include "sabre/builtins/todo/traits.hpp"
#include "sabre/builtins/utility/traits.hpp"
#include "sabre/builtins/void/traits.hpp"

//  X-MACROS  //

#define SABRE_XX_BUILTINS_CUSTOM(X) \
  X(Builtins::Custom::Todo)         \
  X(Builtins::Custom::Debug)        \
  X(Builtins::Custom::Utility)

#define SABRE_XX_BUILTINS_LIST(X) \
  SABRE_XX_VALUES_IMMEDIATE(X)    \
  SABRE_XX_VALUES_OBJECT(X)       \
  X(Object::Instance)             \
  SABRE_XX_BUILTINS_CUSTOM(X)

#endif
