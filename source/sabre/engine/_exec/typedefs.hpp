/// Engine Modules
#include "sabre/engine/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_ENGINE_EXECUTE(TYPE_CAST, isolate, frame, unqualified) {
  // resolve the qualified instruction instance
  auto *instruction = unqualified->cast<Glyph::TYPE_CAST>();

  // get both the value and the guard now
  auto value = frame->load(instruction->get<1>());
  auto guard = frame->load(instruction->get<2>());

  // get the result now
  auto result = ensure(isolate, value, guard);

  // and return the necessary details
  if (!result.pointer().okay()) return result;
  frame->store(instruction->get<0>(), result);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(TYPE_GUARD, isolate, frame, unqualified) {
  // resolve the qualified instruction instance
  auto *instruction = unqualified->cast<Glyph::TYPE_GUARD>();

  // get both the value and the guard now
  auto value = frame->load(instruction->get<1>());
  auto guard = frame->load(instruction->get<2>());

  // check if we have some valid extensions now
  auto success = extends(value, guard) == Subtype::SUCCESS;

  // and resolve the value to be shown now
  frame->store(instruction->get<0>(), Value::Boolean(success));
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}