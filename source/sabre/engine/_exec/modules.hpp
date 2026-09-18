/// Sabre Includes
#include "sabre/engine/exports.hpp"
#include "sabre/member/factory.hpp"

/// Engine Modules
#include "sabre/engine/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_ENGINE_EXECUTE(MODULE_OPEN, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::MODULE_OPEN>();
  auto *exports = isolate->exports(frame->resource().buffer());
  frame->store(instruction->get<0>(), exports->open(isolate));
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(MODULE_CLOSE, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::MODULE_CLOSE>();
  auto *exports = isolate->exports(frame->resource().buffer());
  frame->store(instruction->get<0>(), exports->close());
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(MODULE_BARREL, isolate, frame, unqualified) {
  // prepare the qualified instruction details
  auto *instruction = unqualified->cast<Glyph::MODULE_BARREL>();
  auto object = frame->load<Object::Instance>(instruction->get<0>());

  // attempt running the barrel export now
  auto result = barrel(isolate, frame, object);
  if (!result.pointer().okay()) return result;
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(MODULE_IMPORT, isolate, frame, unqualified) {
  // prepare the qualified instruction
  auto *instruction = unqualified->cast<Glyph::MODULE_IMPORT>();

  // prepare the import details to be used
  auto path = frame->intern(instruction->get<1>()).view();
  auto trace = frame->backtrace(Resource::Group::IMPORT);
  auto value = isolate->import(path, frame->resource().body(), trace);

  // check if the resulting value is good
  if (!value.pointer().okay()) return value;
  frame->store(instruction->get<0>(), value);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(MODULE_EXPORT, isolate, frame, unqualified) {
  // prepare the qualified instruction
  auto *instruction = unqualified->cast<Glyph::MODULE_EXPORT>();

  // get the details that we should use for updating
  auto value = frame->load(instruction->get<0>());
  const auto &name = frame->intern(instruction->get<1>());

  // attempt exporting the current value now
  auto result = expose(isolate, frame, value, &name);
  if (!result.pointer().okay()) return result;
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}
