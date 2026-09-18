/// Sabre Includes
#include "sabre/engine/dispatch.hpp"
#include "sabre/runtime/isolate.hpp"

/// Engine Includes
#include "sabre/engine/_inline/execute.ipp"

//  PUBLIC METHODS  //

$_WILLTAIL Sabre::Value::Any
Sabre::Engine::Dispatch::tailcall(Runtime::Isolate *isolate, Function::Frame *frame, const Instruction *unqualified) {
  // prepare the callback typing to be used
  using Callback = decltype(&tailcall);

  // prepare the available dispatch table now
  static constexpr Callback s_table[] = {
#define SABRE_XX_GLYPH_BASE(N, ...) m_execute<Bytecode::Glyph::N>,
#include "sabre/bytecode/_defines/glyphs.def"
  };

  // update the instruction to be dispatched here now
  unqualified = std::bit_cast<Bytecode::Instruction *>(frame->advance());
  $_MUSTTAIL return s_table[unqualified->glyph().encoded()](isolate, frame, unqualified);
}
