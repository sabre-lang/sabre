#ifndef _SABRE_ENGINE_MACROS_IPP
#define _SABRE_ENGINE_MACROS_IPP

/// Sabre Includes
#include "sabre/engine/dispatch.hpp"

/// Value Includes
#include "sabre/value/_inline/value.ipp"

//  MACROS  //

#define SABRE_MM_ENGINE_EXECUTE(N, T, F, I, ...)                                                             \
  template <>                                                                                                \
  $_WILLTAIL $_INLINE_FORCE Sabre::Value::Any Sabre::Engine::Dispatch::m_execute<Sabre::Bytecode::Glyph::N>( \
      Isolate * T, Function::Frame * F, const Instruction *I                                                 \
  )

#endif
