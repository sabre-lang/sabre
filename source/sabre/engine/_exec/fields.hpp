/// Engine Modules
#include "sabre/engine/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_ENGINE_EXECUTE(LOAD_FIELD, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::LOAD_FIELD>();
  auto target = frame->load(instruction->get<1>());
  auto symbol = frame->constant<Value::Symbol>(instruction->get<2>());
  auto field = getter(isolate, target, symbol);
  if ($_UNLIKELY(!field.pointer().okay())) return field;
  frame->store(instruction->get<0>(), field);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(STORE_FIELD, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::STORE_FIELD>();
  auto value = frame->load(instruction->get<1>());
  auto target = frame->load(instruction->get<0>());
  auto symbol = frame->constant<Value::Symbol>(instruction->get<2>());
  auto field = setter(isolate, target, value, symbol);
  if ($_UNLIKELY(!field.pointer().okay())) return field;
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}
