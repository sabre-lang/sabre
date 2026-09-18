/// Sabre Includes
#include "sabre/member/factory.hpp"

/// Engine Modules
#include "sabre/engine/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_ENGINE_EXECUTE(OBJECT_EMPTY, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::OBJECT_EMPTY>();
  auto object = isolate->create<Object::Instance>();
  frame->store(instruction->get<0>(), object);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(OBJECT_MAKE, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::OBJECT_MAKE>();
  auto value = m_object(isolate, frame->span(instruction->get<1>()));
  frame->store(instruction->get<0>(), value); // store the result
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(OBJECT_ATTR, isolate, frame, unqualified) {
  // get the qualified instruction
  auto *instruction = unqualified->cast<Glyph::OBJECT_ATTR>();

  // resolve the callback to be bound
  auto callback = frame->load(instruction->get<0>());
  auto target = frame->load(instruction->get<1>());

  // resolve the expected kind to be emitted
  auto kind = static_cast<Operator::Kind>(instruction->get<2>().encode());

  // and attempt calling the required instance now
  auto result = overrides(isolate, target, callback, kind);

  // check if the overriden result succeeds or not
  if (!result.pointer().okay()) return result;
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}
