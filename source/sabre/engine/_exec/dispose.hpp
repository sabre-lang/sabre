/// Sabre Includes
#include "sabre/garbage/service.hpp"

/// Engine Modules
#include "sabre/engine/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_ENGINE_EXECUTE(DISPOSE_OPEN, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::DISPOSE_OPEN>();
  isolate->lifetimes()->open(isolate, instruction->get<0>());
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(DISPOSE_CLOSE, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::DISPOSE_OPEN>();
  if (!isolate->lifetimes()->close(isolate, instruction->get<0>())) return Value::Failure();
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(DISPOSE_TRACE, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::DISPOSE_TRACE>();
  isolate->lifetimes()->defer(isolate, frame->load(instruction->get<0>()));
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(DEFER_0_VOID, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::DEFER_0_VOID>();
  auto result = defer(isolate, frame->accumulator());
  if ($_UNLIKELY(!result.pointer().okay())) return result;
  frame->store(instruction->get<0>(), result);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(DEFER_N_VOID, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::DEFER_N_VOID>();
  auto [callee, argv] = frame->split(instruction->get<1>());
  auto result = defer(isolate, callee, argv);
  if ($_UNLIKELY(!result.pointer().okay())) return result;
  frame->store(instruction->get<0>(), result);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(DEFER_0_FIELD, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::DEFER_0_FIELD>();
  auto symbol = frame->constant<Value::Symbol>(instruction->get<1>());
  auto result = m_defer(isolate, symbol, {frame->accumulator()});
  if ($_UNLIKELY(!result.pointer().okay())) return result;
  frame->store(instruction->get<0>(), result);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(DEFER_N_FIELD, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::DEFER_N_FIELD>();
  auto [target, argv] = frame->split(instruction->get<2>());
  auto symbol = frame->constant<Value::Symbol>(instruction->get<1>());
  auto result = m_defer(isolate, symbol, {target, argv});
  if ($_UNLIKELY(!result.pointer().okay())) return result;
  frame->store(instruction->get<0>(), result);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}
