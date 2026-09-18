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

SABRE_MM_ENGINE_EXECUTE(DISPOSE_DEFER, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::DISPOSE_DEFER>();
  isolate->lifetimes()->defer(isolate, frame->load(instruction->get<0>()));
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}
