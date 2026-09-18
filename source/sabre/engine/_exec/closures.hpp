/// Sabre Includes
#include "sabre/engine/invoke.hpp"
#include "sabre/machine/service.hpp"

/// Engine Modules
#include "sabre/engine/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_ENGINE_EXECUTE(CLOSURE_MAKE, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::CLOSURE_MAKE>();
  auto *info = frame->arena()->functions.at(instruction->get<1>()).get();
  auto value = isolate->create<Function::Closure>(info, frame->self(), frame->context());
  frame->store(instruction->get<0>(), value); // and store the result value now
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(CLOSURE_LIFT, isolate, frame, unqualified) {
  // get the qualified instruction
  auto *instruction = unqualified->cast<Glyph::CLOSURE_LIFT>();

  // prepare the slot/target values
  auto slot = instruction->get<0>();
  auto target = frame->load(slot);

  // check if we have a closure value at all
  if (!target.is<Function::Closure>()) $_MUSTTAIL return tailcall(isolate, frame, unqualified);

  // pull out the information to be used now
  auto closure = target.as<Function::Closure>();

  // attempt compiling the incoming target
  auto *machine = isolate->service<Machine::Service>();
  frame->store(slot, machine->upgrade(isolate, closure));
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(CLOSURE_PASS, isolate, frame, unqualified) {
  // prepare the incoming instruction
  auto *instruction = unqualified->cast<Glyph::CLOSURE_PASS>();

  // get the associated context
  const auto &context = frame->context();

  // prepare the self and callee values
  auto receiver = context.load(0), callee = context.load(1);

  // attempt calling the instance
  auto result = invoke(isolate, callee, {receiver, frame->argv()});
  if ($_UNLIKELY(!result.pointer().okay())) return result;
  frame->store(instruction->get<0>(), result);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(CALL_0_VOID, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::CALL_0_VOID>();
  auto result = invoke(isolate, frame->accumulator());
  if ($_UNLIKELY(!result.pointer().okay())) return result;
  frame->store(instruction->get<0>(), result);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(CALL_N_VOID, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::CALL_N_VOID>();
  auto [callee, argv] = frame->split(instruction->get<1>());
  auto result = invoke(isolate, callee, argv);
  if ($_UNLIKELY(!result.pointer().okay())) return result;
  frame->store(instruction->get<0>(), result);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(CALL_0_INLINE, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::CALL_0_INLINE>();
  auto result = Invoke::inlined(isolate, frame); // attempt a recall
  if ($_UNLIKELY(!result.pointer().okay())) return result;
  frame->store(instruction->get<0>(), result);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(CALL_N_INLINE, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::CALL_N_INLINE>();
  auto result = Invoke::inlined(isolate, frame, frame->span(instruction->get<1>()));
  if ($_UNLIKELY(!result.pointer().okay())) return result;
  frame->store(instruction->get<0>(), result);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(CALL_0_FIELD, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::CALL_0_FIELD>();
  auto symbol = frame->constant<Value::Symbol>(instruction->get<1>());
  auto result = m_invoke(isolate, symbol, {frame->accumulator()});
  if ($_UNLIKELY(!result.pointer().okay())) return result;
  frame->store(instruction->get<0>(), result);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(CALL_N_FIELD, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::CALL_N_FIELD>();
  auto [target, argv] = frame->split(instruction->get<2>());
  auto symbol = frame->constant<Value::Symbol>(instruction->get<1>());
  auto result = m_invoke(isolate, symbol, {target, argv});
  if ($_UNLIKELY(!result.pointer().okay())) return result;
  frame->store(instruction->get<0>(), result);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(SPAWN_0_VOID, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::CALL_0_VOID>();
  auto result = spawn(isolate, frame->accumulator());
  if ($_UNLIKELY(!result.pointer().okay())) return result;
  frame->store(instruction->get<0>(), result);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(SPAWN_N_VOID, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::CALL_N_VOID>();
  auto [callee, argv] = frame->split(instruction->get<1>());
  auto result = spawn(isolate, callee, argv);
  if ($_UNLIKELY(!result.pointer().okay())) return result;
  frame->store(instruction->get<0>(), result);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(SPAWN_0_FIELD, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::CALL_0_FIELD>();
  auto symbol = frame->constant<Value::Symbol>(instruction->get<1>());
  auto result = m_invoke(isolate, symbol, {frame->accumulator()});
  if ($_UNLIKELY(!result.pointer().okay())) return result;
  frame->store(instruction->get<0>(), result);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(SPAWN_N_FIELD, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::CALL_N_FIELD>();
  auto [target, argv] = frame->split(instruction->get<2>());
  auto symbol = frame->constant<Value::Symbol>(instruction->get<1>());
  auto result = m_spawn(isolate, symbol, {target, argv});
  if ($_UNLIKELY(!result.pointer().okay())) return result;
  frame->store(instruction->get<0>(), result);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}
