/// Engine Modules
#include "sabre/engine/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_ENGINE_EXECUTE(LOAD_CONTEXT, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::LOAD_CONTEXT>();
  auto value = frame->context().load(instruction->get<1>());
  frame->store(instruction->get<0>(), value);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(STORE_CONTEXT, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::STORE_CONTEXT>();
  auto value = frame->load(instruction->get<1>());
  frame->context().store(instruction->get<0>(), value);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(LOAD_UPVALUE, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::LOAD_UPVALUE>();
  auto context = frame->context().parent(instruction->get<2>());
  frame->store(instruction->get<0>(), context.load(instruction->get<1>()));
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(STORE_UPVALUE, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::LOAD_UPVALUE>();
  auto context = frame->context().parent(instruction->get<2>());
  context.store(instruction->get<0>(), frame->load(instruction->get<1>()));
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}
