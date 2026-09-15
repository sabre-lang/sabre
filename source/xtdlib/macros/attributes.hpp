#ifndef _XTDLIB_MACROS_ATTRIBUTES_HPP
#define _XTDLIB_MACROS_ATTRIBUTES_HPP

/// Library Includes
#include "xtdlib/macros/processor.hpp"

//  MACROS  //

#define $_AUTO auto $_PP_UNIQUE()
#define $_UNUSED [[maybe_unused]]

#define $_NORETURN [[noreturn]]
#define $_NODISCARD [[nodiscard]]
#define $_FALLTHROUGH [[fallthrough]]
#define $_DEPRECATED(...) [[deprecated(__VA_ARGS__)]]

#ifndef __has_feature
#define __has_feature(...) 0
#endif

#if __has_feature(address_sanitizer) || defined(__SANITIZE_ADDRESS__)
#define $_ASAN_ENABLED 1
#include <sanitizer/asan_interface.h>
#else
#define $_ASAN_ENABLED 0
#endif

#if __has_feature(thread_sanitizer) || defined(__SANITIZE_THREAD__)
#define $_TSAN_ENABLED 1
#else
#define $_TSAN_ENABLED 0
#endif

#if !$_PP_NDEBUG
#define $_INLINE_PERF
#define $_INLINE_ALWAYS
#else
#define $_INLINE_PERF inline $_INLINE_ALWAYS
#if defined(__has_attribute) && __has_attribute(always_inline)
#define $_INLINE_ALWAYS __attribute__((always_inline))
#elif defined(__forceinline)
#define $_INLINE_ALWAYS __forceinline
#else
#define $_INLINE_ALWAYS
#endif
#endif

#define $_ALIGNAS(N) alignas(N)
#define $_ALIGNAS_CACHE() $_ALIGNAS(std::hardware_destructive_interference_size)
#define $_INLINE_FORCE inline $_INLINE_ALWAYS

#if defined(__clang__) && !$_ASAN_ENABLED
#define $_MUSTTAIL [[clang::musttail]]
#define $_WILLTAIL [[clang::preserve_none]]
#else
#define $_MUSTTAIL
#define $_WILLTAIL
#endif

#if defined(__has_builtin) && __has_builtin(__builtin_expect)
#define $_LIKELY(X) __builtin_expect(!!(X), 1)
#define $_UNLIKELY(X) __builtin_expect(!!(X), 0)
#else
#define $_LIKELY(X) (X)
#define $_UNLIKELY(X) (X)
#endif

#if defined(__has_declspec_attribute) && __has_declspec_attribute(novtable)
#define $_ABSTRACT __declspec(novtable)
#else
#define $_ABSTRACT
#endif

#endif
