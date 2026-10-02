#ifndef _XTDLIB_SYSTEM_UNREACHABLE_HPP
#define _XTDLIB_SYSTEM_UNREACHABLE_HPP

/// Library Includes
#include "xtdlib/macros/attributes.hpp"
#include "xtdlib/system/abort.hpp"

namespace $::System {

//  PUBLIC METHODS  //

#if __cplusplus >= 202302L
$_NORETURN $_INLINE_FORCE static void unreachable() { std::unreachable(); }
#elif defined(__has_builtin) && __has_builtin(__builtin_unreachable)
$_NORETURN $_INLINE_FORCE static void unreachable() { __builtin_unreachable(); }
#elif defined(__assume)
$_NORETURN $_INLINE_FORCE static void unreachable() { __assume(false); }
#else
$_NORETURN $_INLINE_FORCE static void unreachable() { abort(); }
#endif

} // namespace $::System

#endif
