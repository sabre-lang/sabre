/// Engine Modules
#include "sabre/engine/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_ENGINE_EXECUTE(REG_MOVE, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::REG_MOVE>();
  frame->move(instruction->get<1>(), instruction->get<0>());
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(REG_SWAP, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::REG_MOVE>();
  frame->swap(instruction->get<0>(), instruction->get<1>());
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}
