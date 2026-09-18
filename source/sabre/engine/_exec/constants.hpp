/// Engine Modules
#include "sabre/engine/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_ENGINE_EXECUTE(LOAD_ZERO, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::LOAD_ZERO>();
  frame->store(instruction->get<0>(), Number::Zero);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(LOAD_ONE, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::LOAD_ONE>();
  frame->store(instruction->get<0>(), Number::One);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(LOAD_VOID, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::LOAD_VOID>();
  frame->store(instruction->get<0>(), Value::Void());
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(LOAD_TRUE, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::LOAD_TRUE>();
  frame->store(instruction->get<0>(), Value::True);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(LOAD_FALSE, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::LOAD_FALSE>();
  frame->store(instruction->get<0>(), Value::False);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(LOAD_SELF, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::LOAD_SELF>();
  frame->store(instruction->get<0>(), frame->self());
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(LOAD_CONST, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::LOAD_CONST>();
  frame->store(instruction->get<0>(), frame->constant(instruction->get<1>()));
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(LOAD_GLOBAL, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::LOAD_GLOBAL>();
  auto symbol = frame->constant<Value::Symbol>(instruction->get<1>());
  frame->store(instruction->get<0>(), isolate->global(symbol));
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}
