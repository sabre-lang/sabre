/// Engine Modules
#include "sabre/engine/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_ENGINE_EXECUTE(STRING_MAKE, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::STRING_MAKE>();
  const auto &intern = frame->intern(instruction->get<1>());
  frame->store(instruction->get<0>(), String::Any(isolate, intern));
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(STRING_CONCAT, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::STRING_CONCAT>();
  auto left = frame->load(instruction->get<1>());
  auto right = frame->load(instruction->get<2>());
  frame->store(instruction->get<0>(), concat(isolate, left, right));
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}
