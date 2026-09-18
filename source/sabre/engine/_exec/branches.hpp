/// Engine Modules
#include "sabre/engine/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_ENGINE_EXECUTE(JUMP_TPL, , , ) { $::System::unreachable(); }

SABRE_MM_ENGINE_EXECUTE(JUMP_TO, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::JUMP_TO>();
  auto interrupt = m_jump(frame, instruction->get<0>());
  if (interrupt) return m_interrupt(isolate, frame);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(JUMP_FILLED, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::JUMP_FILLED>();
  auto filled = !frame->load(instruction->get<1>()).is<Value::Void>();
  auto interrupt = filled && m_jump(frame, instruction->get<0>());
  if (interrupt) return m_interrupt(isolate, frame);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(JUMP_TRUTHY, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::JUMP_TRUTHY>();
  auto jump = frame->load(instruction->get<1>()).truthiness();
  auto interrupt = jump && m_jump(frame, instruction->get<0>());
  if (interrupt) return m_interrupt(isolate, frame);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(JUMP_FALSEY, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::JUMP_FALSEY>();
  auto jump = !frame->load(instruction->get<1>()).truthiness();
  auto interrupt = jump && m_jump(frame, instruction->get<0>());
  if (interrupt) return m_interrupt(isolate, frame);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(MATCH_VOID, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::MATCH_VOID>();
  auto jump = frame->load(instruction->get<1>()) == Value::Void();
  auto interrupt = jump && m_jump(frame, instruction->get<0>());
  if (interrupt) return m_interrupt(isolate, frame);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(MATCH_TRUE, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::MATCH_TRUE>();
  auto jump = frame->load(instruction->get<1>()) == Value::True;
  auto interrupt = jump && m_jump(frame, instruction->get<0>());
  if (interrupt) return m_interrupt(isolate, frame);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(MATCH_FALSE, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::MATCH_FALSE>();
  auto jump = frame->load(instruction->get<1>()) == Value::False;
  auto interrupt = jump && m_jump(frame, instruction->get<0>());
  if (interrupt) return m_interrupt(isolate, frame);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(MATCH_TEXT, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::MATCH_TEXT>();
  auto value = frame->load(instruction->get<1>());
  const auto &intern = frame->intern(instruction->get<2>());
  auto jump = value.is<String::Any>() && !value.as<String::Any>().compare(intern.view());
  auto interrupt = jump && m_jump(frame, instruction->get<0>());
  if (interrupt) return m_interrupt(isolate, frame);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(MATCH_CONST, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::MATCH_CONST>();
  auto value = frame->load(instruction->get<1>());
  auto constant = frame->constant(instruction->get<2>());
  auto interrupt = value == constant && m_jump(frame, instruction->get<0>());
  if (interrupt) return m_interrupt(isolate, frame);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(MATCH_GUARD, isolate, frame, unqualified) {
  // resolve the qualified instruction instance
  auto *instruction = unqualified->cast<Glyph::MATCH_GUARD>();

  // get both the value and the guard now
  auto value = frame->load(instruction->get<1>());
  auto guard = frame->load(instruction->get<2>());

  auto jump = matches(value, guard); // attempt matching now
  auto interrupt = jump && m_jump(frame, instruction->get<0>());
  if (interrupt) return m_interrupt(isolate, frame);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}
