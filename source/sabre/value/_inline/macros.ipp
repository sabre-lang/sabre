#ifndef _SABRE_VALUE_MACROS_IPP
#define _SABRE_VALUE_MACROS_IPP

/// Sabre Includes
#include "sabre/value/pointer.hpp"

/// Value Includes
#include "sabre/forward/async.hpp"
#include "sabre/forward/function.hpp"
#include "sabre/forward/iterable.hpp"
#include "sabre/forward/monad.hpp"
#include "sabre/forward/number.hpp"
#include "sabre/forward/object.hpp"
#include "sabre/forward/runtime.hpp"
#include "sabre/forward/string.hpp"

//  X-MACROS  //

#define SABRE_XX_VALUES_IMMEDIATE(X) \
  X(Value::Void)                     \
  X(Value::Symbol)                   \
  X(Value::Boolean)                  \
  X(Number::Tagged)

#define SABRE_XX_VALUES_OBJECT(X) \
  X(String::Any)                  \
                                  \
  X(Function::Native)             \
  X(Function::Jitted)             \
  X(Function::Closure)            \
                                  \
  X(Object::Enum)                 \
  X(Object::Class)                \
  X(Object::Exception)            \
                                  \
  X(Async::Future)                \
                                  \
  X(Iterable::Iterator)           \
  X(Iterable::List)               \
                                  \
  X(Monad::Result)

#define SABRE_XX_VALUES_LIST(X) \
  SABRE_XX_VALUES_IMMEDIATE(X)  \
  X(String::Small)              \
                                \
  X(String::Large)              \
  SABRE_XX_VALUES_OBJECT(X)     \
  X(Object::Instance)

#endif
