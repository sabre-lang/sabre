/// Sabre Includes
#include "sabre/engine/invoke.hpp"
#include "sabre/machine/service.hpp"

/// Engine Modules
#include "sabre/engine/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_ENGINE_EXECUTE(ASYNC_0_VOID, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::ASYNC_0_VOID>();
  auto result = async(isolate, frame->accumulator());
  if ($_UNLIKELY(!result.pointer().okay())) return result;
  frame->store(instruction->get<0>(), result);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(ASYNC_N_VOID, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::ASYNC_N_VOID>();
  auto [callee, argv] = frame->split(instruction->get<1>());
  auto result = async(isolate, callee, argv);
  if ($_UNLIKELY(!result.pointer().okay())) return result;
  frame->store(instruction->get<0>(), result);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(ASYNC_0_FIELD, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::ASYNC_0_FIELD>();
  auto symbol = frame->constant<Value::Symbol>(instruction->get<1>());
  auto result = m_async(isolate, symbol, {frame->accumulator()});
  if ($_UNLIKELY(!result.pointer().okay())) return result;
  frame->store(instruction->get<0>(), result);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(ASYNC_N_FIELD, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::ASYNC_N_FIELD>();
  auto [target, argv] = frame->split(instruction->get<2>());
  auto symbol = frame->constant<Value::Symbol>(instruction->get<1>());
  auto result = m_async(isolate, symbol, {target, argv});
  if ($_UNLIKELY(!result.pointer().okay())) return result;
  frame->store(instruction->get<0>(), result);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}
