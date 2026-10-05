/// Machine Includes
#include "sabre/machine/_inline/macros.ipp"

//  EMITTER METHODS  //

SABRE_MM_MACHINE_EMIT(DISPOSE_OPEN, builder, instruction) {
  auto depth = instruction->get<0>().encode();
  __ee__ call(Glue::watch, builder->isolate, depth);
}

SABRE_MM_MACHINE_EMIT(DISPOSE_CLOSE, builder, instruction) {
  auto dx = __cc__ new_gp64();
  auto depth = instruction->get<0>().encode();
  __ee__ call(Glue::ignore, dx, builder->isolate, depth);
  __ee__ test(dx, Validate::FAST); // validate the result
}

SABRE_MM_MACHINE_EMIT(DISPOSE_TRACE, builder, instruction) {
  auto tx = __ee__ slot(instruction->get<0>());
  __ee__ call(Glue::trace, builder->isolate, tx);
}

SABRE_MM_MACHINE_EMIT(DEFER_0_VOID, builder, instruction) {
  // prepare the register slots we require
  auto dx = __ee__ slot(instruction->get<0>());
  auto tx = __ee__ slot(Register::Accumulator);

  // we define an empty set of parameters
  __ee__ params();

  // and then start calling the necessary glue method
  __ee__ call(Glue::defer, dx, builder->isolate, tx, builder->params);

  // finally do a fast test after the invocation
  __ee__ test(dx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(DEFER_N_VOID, builder, instruction) {
  // prepare the span to be used
  auto span = instruction->get<1>();

  // prepare the register slots we require
  auto tx = __ee__ slot(span.first());
  auto dx = __ee__ slot(instruction->get<0>());

  // we define a baseline set of parameters
  __ee__ params(span.slice(1));

  // and then start calling the necessary glue method
  __ee__ call(Glue::defer, dx, builder->isolate, tx, builder->params);

  // finally do a fast test after the invocation
  __ee__ test(dx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(DEFER_0_FIELD, builder, instruction) {
  // prepare the arena details to be used
  auto index = instruction->get<1>();

  // prepare the register slots we require
  auto vx = __cc__ new_gp64();
  auto dx = __ee__ slot(instruction->get<0>());
  auto tx = __ee__ slot(Register::Accumulator);

  // start by getting the required field here
  __ee__ getter(vx, tx, index);

  // we define a baseline set of parameters
  __ee__ params(Register::Accumulator);

  // and then start calling the necessary glue method
  __ee__ call(Glue::defer, dx, builder->isolate, vx, builder->params);

  // finally do a fast test after the invocation
  __ee__ test(dx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(DEFER_N_FIELD, builder, instruction) {
  // prepare the arena details to be used
  auto index = instruction->get<1>();
  auto span = instruction->get<2>();

  // prepare the register slots we require
  auto vx = __cc__ new_gp64();
  auto tx = __ee__ slot(span.first());
  auto dx = __ee__ slot(instruction->get<0>());

  // start by getting the required field here
  __ee__ getter(vx, tx, index);

  // we define a baseline set of parameters
  __ee__ params(span.first(), span.slice(1));

  // and then start calling the necessary glue method
  __ee__ call(Glue::defer, dx, builder->isolate, vx, builder->params);

  // finally do a fast test after the invocation
  __ee__ test(dx, Validate::FAST);
}
