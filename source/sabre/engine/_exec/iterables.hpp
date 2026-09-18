/// Engine Modules
#include "sabre/engine/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_ENGINE_EXECUTE(LIST_EMPTY, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::LIST_EMPTY>();
  frame->store(instruction->get<0>(), isolate->create<Iterable::List>());
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(LIST_MAKE, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::LIST_MAKE>();
  auto elements = frame->span(instruction->get<1>());
  frame->store(instruction->get<0>(), isolate->create<Iterable::List>(elements));
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(ITER_LOAD, isolate, frame, unqualified) {
  // prepare the qualified instruction and iterable
  auto *instruction = unqualified->cast<Glyph::ITER_LOAD>();
  auto iterable = frame->load(instruction->get<1>());

  // attempt resolving the iterator now
  auto value = iterator(isolate, iterable);
  if (!value.pointer().okay()) return value;

  // and save the final result to be used
  frame->store(instruction->get<0>(), value);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(ITER_NEXT, isolate, frame, unqualified) {
  // prepare the qualified instruction
  auto *instruction = unqualified->cast<Glyph::ITER_NEXT>();

  // get some details about incoming instruction
  auto slot = instruction->get<0>();
  auto value = instruction->get<1>();
  auto index = instruction->get<2>();

  auto iterator = frame->load<Iterable::Iterator>(slot);
  auto result = iterator.done() || !iterator.okay();
  if (!result) result = iterator.next(isolate);
  if (!iterator.okay()) return iterator;

  // update the current details to be used
  if (!value.nowhere()) frame->store(value, iterator.value());
  if (!index.nowhere()) frame->store(index, Number::Tagged(iterator.index() - 1));

  // and allow jumping based on the done-state
  frame->accumulator() = Value::Boolean(result);

  // tail to the next instruction now
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}
