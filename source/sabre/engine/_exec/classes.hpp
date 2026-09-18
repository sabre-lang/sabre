/// Sabre Includes
#include "sabre/engine/invoke.hpp"
#include "sabre/member/factory.hpp"

/// Engine Modules
#include "sabre/engine/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_ENGINE_EXECUTE(CLASS_BIND, isolate, frame, unqualified) {
  // prepare the qualified instruction
  auto *instruction = unqualified->cast<Glyph::CLASS_BIND>();

  // attempt resolving the incoming class prototype now
  auto prototype = frame->load<Object::Class>(instruction->get<0>());

  // resolve the underlying constructor and bind it to the class
  auto *info = frame->arena()->functions.at(instruction->get<1>()).get();

  // attempt binding the required constructor to the prototype
  auto result = constructor(isolate, prototype, info, frame->context());
  if (!result.pointer().okay()) return result; // fail if binding failed

  // successfully bound and field so we return now
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(CLASS_MAKE, isolate, frame, unqualified) {
  // prepare the qualified instruction
  auto *instruction = unqualified->cast<Glyph::CLASS_MAKE>();

  // pull out the underlying slot and parent
  auto slot = instruction->get<0>();
  auto super = frame->load(slot);

  // then also resolve the name and shape
  Shape::Underlying shape = instruction->get<2>();
  const auto &name = frame->intern(instruction->get<1>());

  // and bind the prototype to the extension output
  frame->store(slot, isolate->create<Object::Class>(name.view(), shape, super));

  // successfully bound and field so we return now
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(CLASS_SUPER, isolate, frame, unqualified) {
  // prepare the qualified instruction
  auto *instruction = unqualified->cast<Glyph::CLASS_SUPER>();

  // get the incoming class instance to be constructed
  auto span = frame->span(instruction->get<1>());
  auto instance = frame->self().as<Object::Instance>();

  // attempt calling our super constructor now
  auto result = Engine::Dispatch::super(isolate, instance, {instance, span});
  if (!result.pointer().okay()) return result; // bail if result failed

  // successfully bound and field so we return now
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(CLASS_EXPORT, isolate, frame, unqualified) {
  // prepare the qualified instruction
  auto *instruction = unqualified->cast<Glyph::CLASS_EXPORT>();

  // get the underlying name and value now
  auto value = frame->load(instruction->get<0>());
  auto instance = frame->self().as<Object::Instance>();
  const auto &name = frame->intern(instruction->get<1>());

  // attempt running our member binder now
  auto result = member(isolate, instance, &name, value, instruction->get<2>());
  if (!result.pointer().okay()) return result; // failed binding the member export

  // successfully bound and field so we return now
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}
