#ifndef _SABRE_DYLIB_INLINE_IPP
#define _SABRE_DYLIB_INLINE_IPP

/// Sabre Includes
#include "sabre/dylib/registry.hpp"
#include "sabre/member/factory.hpp"

/// Builtins Includes
#include "sabre/builtins/_inline/assert.ipp"

//  MACROS  //

/// @brief Allows unwrapping dynamic library exports.
#define SABRE_MM_DYLIB_UNWRAP(N, ...)                                                  \
  exports.fields().emplace(#N, Member::Factory::native(isolate, m_##N, m_name(), #N));

/// @brief Exposes a dynamic library addon.
#define SABRE_MM_DYLIB_ADDON(P, L, ...)                                                                 \
  __attribute((used)) static const $_AUTO = Sabre::Dylib::Registry::install<Sabre::Package::P>();       \
  Sabre::Package::P::P(Runtime::Isolate *isolate, Dylib::Exports &exports) { L(SABRE_MM_DYLIB_UNWRAP) }

/// @brief Exposes a dynamic library method.
#define SABRE_MM_DYLIB_DEFINE(N, ...)                                                                     \
  static Value::Any m_##N(Runtime::Isolate *isolate, const Function::Args &args $_PP_VARGS(__VA_ARGS__));

/// @brief Exposes a dynamic library method.
#define SABRE_MM_DYLIB_METHOD(P, N, I, A, ...)                                                                     \
  Sabre::Value::Any Sabre::Package::P::m_##N(Runtime::Isolate *I, const Function::Args &A $_PP_VARGS(__VA_ARGS__))

#endif
