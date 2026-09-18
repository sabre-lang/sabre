/// Machine Includes
#include "sabre/machine/_inline/macros.ipp"

/// Forward Declarations
$_FWD(Sabre::Machine::Glue, Reference value(Reference))
$_FWD(Sabre::Machine::Glue, Reference index(Reference))
$_FWD(Sabre::Machine::Glue, Reference bump(Runtime::Isolate *, Reference))

//  PUBLIC METHODS  //

Sabre::Machine::Reference Sabre::Machine::Glue::value(Reference iterable) {
  return Value::Cast<Iterable::Iterator>(iterable).value().pointer();
}

Sabre::Machine::Reference Sabre::Machine::Glue::index(Reference iterable) {
  return Number::Tagged(Value::Cast<Iterable::Iterator>(iterable).index() - 1).pointer();
}

Sabre::Machine::Reference Sabre::Machine::Glue::bump(Runtime::Isolate *isolate, Reference iterable) {
  // load the incoming iterator to be bumped
  auto iterator = Value::Cast<Iterable::Iterator>(iterable);

  // get the resulting and handle as needed
  auto result = iterator.done() || !iterator.okay();
  if (!result) result = iterator.next(isolate);

  // and return a suitable result now as needed
  return iterator.okay() ? Value::Boolean(result).pointer() : Value::Failure().pointer();
}

//  EMITTER METHODS  //

SABRE_MM_MACHINE_EMIT(LIST_EMPTY, builder, instruction) {
  auto dx = __ee__ slot(instruction->get<0>()); // prepare dest
  __ee__ call(Glue::empty<Iterable::List>, dx, builder->isolate);
}

SABRE_MM_MACHINE_EMIT(LIST_MAKE, builder, instruction) {
  // prepare the destination register
  auto dx = __ee__ slot(instruction->get<0>());

  // ensure we preload the necessary parameters
  __ee__ params(instruction->get<1>());

  // and invoke the enumeration constructor
  __ee__ call(Glue::list, dx, builder->isolate, builder->params);
}

SABRE_MM_MACHINE_EMIT(ITER_LOAD, builder, instruction) {
  // prepare the required registers
  auto dx = __ee__ slot(instruction->get<0>());
  auto tx = __ee__ slot(instruction->get<1>());

  // attempt resolving the required iterator value
  __ee__ call(Glue::iterator, dx, builder->isolate, tx);

  // ensure that the output iterator is valid
  __ee__ test(dx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(ITER_NEXT, builder, instruction) {
  // prepare the slots that we could be using
  auto value = instruction->get<1>();
  auto index = instruction->get<2>();

  // prepare the required registers to be used
  auto dx = __cc__ new_gp64();
  auto tx = __ee__ slot(instruction->get<0>());

  // attempt bumping the current iterator now
  __ee__ call(Glue::bump, dx, builder->isolate, tx);
  __ee__ test(dx, Validate::FAST); // validate output

  // finally update our resulting value/index slots
  if (!value.nowhere()) __ee__ call(Glue::value, __ee__ slot(value), tx);
  if (!index.nowhere()) __ee__ call(Glue::index, __ee__ slot(index), tx);

  // bind the final result now
  __ee__ move(Register::Accumulator, dx);
}
