#ifndef _SABRE_BUILTINS_ASSERT_IPP
#define _SABRE_BUILTINS_ASSERT_IPP

/// Sabre Includes
#include "sabre/member/factory.hpp"
#include "sabre/number/math.hpp"
#include "sabre/operator/storage.hpp"
#include "sabre/runtime/isolate.hpp"

/// Builtin Includes
#include "sabre/builtins/_inline/builtins.ipp"

/// Value Includes
#include "sabre/value/_inline/value.ipp"

//  MACROS  //

#define SABRE_MM_ASSERT_THAT(I, C, ...)                 \
  $_PP_BLOCK({                                          \
    if ($_UNLIKELY(!(C))) return I->panic(__VA_ARGS__); \
  })

#define SABRE_MM_ASSERT_ARGC(I, S, MIN, ...) SABRE_MM_ASSERT_THAT(I, S >= MIN, 6000251, MIN)
#define SABRE_MM_ASSERT_TYPEOF(I, T, V, ...) SABRE_MM_ASSERT_THAT(I, (V).is<T>(), 6000253, #V, Value::Inspect<T>::name())
#define SABRE_MM_ASSERT_INTEGRAL(I, V, ...) SABRE_MM_ASSERT_THAT(I, Number::Math::integral(V), 6000501)
#define SABRE_MM_ASSERT_INDEX(I, S, IDX, ...) SABRE_MM_ASSERT_THAT(I, (IDX >= 0) && (IDX < S), 3000701, S, IDX)
#define SABRE_MM_ASSERT_LOWER(I, V, MIN, ...) SABRE_MM_ASSERT_THAT(I, V >= MIN, 6000602, MIN)
#define SABRE_MM_ASSERT_UPPER(I, V, MAX, ...) SABRE_MM_ASSERT_THAT(I, V <= MAX, 6000601, MAX)
#define SABRE_MM_ASSERT_BOUNDS(I, V, MIN, MAX, ...) \
  SABRE_MM_ASSERT_LOWER(I, V, MIN);                 \
  SABRE_MM_ASSERT_UPPER(I, V, MAX)

#endif
