/// Sabre Modules
#include "sabre/signal/service.hpp"

/// Engine Modules
#include "sabre/engine/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_ENGINE_EXECUTE(EXEC_INVALID, , , ) { $::System::unreachable(); }

SABRE_MM_ENGINE_EXECUTE(EXEC_NOOP, isolate, frame, unqualified) {
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(EXEC_RETURN, , frame, ) { return frame->accumulator(); }
SABRE_MM_ENGINE_EXECUTE(EXEC_CANCEL, isolate, frame, ) { return m_interrupt(isolate, frame); }
SABRE_MM_ENGINE_EXECUTE(EXEC_PANIC, isolate, frame, ) { return isolate->panic(frame->accumulator()); }

SABRE_MM_ENGINE_EXECUTE(EXEC_ABORT, isolate, , unqualified) {
  auto *instruction = unqualified->cast<Glyph::EXEC_ABORT>();
  isolate->thread()->shutdown(instruction->get<0>()); // bail
}

SABRE_MM_ENGINE_EXECUTE(EXEC_RAISE, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::EXEC_RAISE>();
  auto code = static_cast<Signal::Symbolic>(instruction->get<0>().encode());
  isolate->service<Signal::Service>()->raise(code);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}
