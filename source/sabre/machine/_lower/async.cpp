/// Machine Includes
#include "sabre/machine/_inline/macros.ipp"

//  EMITTER METHODS  //

SABRE_MM_MACHINE_EMIT(ASYNC_0_VOID, builder, instruction) {
  // prepare the register slots we require
  auto dx = __ee__ slot(instruction->get<0>());
  auto tx = __ee__ slot(Register::Accumulator);

  // we define an empty set of parameters
  __ee__ params();

  // and then start calling the necessary glue method
  __ee__ call(Glue::async, dx, builder->isolate, tx, builder->params);

  // finally do a fast test after the invocation
  __ee__ test(dx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(ASYNC_N_VOID, builder, instruction) {
  // prepare the span to be used
  auto span = instruction->get<1>();

  // prepare the register slots we require
  auto tx = __ee__ slot(span.first());
  auto dx = __ee__ slot(instruction->get<0>());

  // we define a baseline set of parameters
  __ee__ params(span.slice(1));

  // and then start calling the necessary glue method
  __ee__ call(Glue::async, dx, builder->isolate, tx, builder->params);

  // finally do a fast test after the invocation
  __ee__ test(dx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(ASYNC_0_FIELD, builder, instruction) {
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
  __ee__ call(Glue::async, dx, builder->isolate, vx, builder->params);

  // finally do a fast test after the invocation
  __ee__ test(dx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(ASYNC_N_FIELD, builder, instruction) {
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
  __ee__ call(Glue::async, dx, builder->isolate, vx, builder->params);

  // finally do a fast test after the invocation
  __ee__ test(dx, Validate::FAST);
}
