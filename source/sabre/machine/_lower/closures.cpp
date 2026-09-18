/// Machine Includes
#include "sabre/machine/_inline/macros.ipp"

//  EMITTER METHODS  //

SABRE_MM_MACHINE_EMIT(CLOSURE_MAKE, builder, instruction) {
  // prepare the function information to be encoded
  auto index = instruction->get<1>();
  auto *arena = builder->info->arena();
  auto &info = arena->functions.at(index);

  // prepare the output destination register
  auto dx = __ee__ slot(instruction->get<0>());

  // request constructing the closure as necessary
  __ee__ call(Glue::closure, dx, builder->isolate, builder->frame, Immediate(info.get()));
}

SABRE_MM_MACHINE_EMIT(CLOSURE_LIFT, builder, instruction) {
  auto dx = __ee__ slot(instruction->get<0>());
  __ee__ call(Glue::upgrade, dx, builder->isolate, dx);
}

SABRE_MM_MACHINE_EMIT(CLOSURE_PASS, builder, instruction) {
  // prepare the output destination register
  auto dx = __ee__ slot(instruction->get<0>());

  // attempt passing the invocation onwards
  __ee__ call(Glue::pass, dx, builder->isolate, builder->frame);

  // test the outgoing result is safe to be returned
  __ee__ test(dx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(CALL_0_VOID, builder, instruction) {
  // prepare the register slots we require
  auto dx = __ee__ slot(instruction->get<0>());
  auto tx = __ee__ slot(Register::Accumulator);

  // we define an empty set of parameters
  __ee__ params();

  // and then start calling the necessary glue method
  __ee__ call(Glue::invoke, dx, builder->isolate, tx, builder->params);

  // finally do a fast test after the invocation
  __ee__ test(dx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(CALL_N_VOID, builder, instruction) {
  // prepare the span to be used
  auto span = instruction->get<1>();

  // prepare the register slots we require
  auto tx = __ee__ slot(span.first());
  auto dx = __ee__ slot(instruction->get<0>());

  // we define a baseline set of parameters
  __ee__ params(span.slice(1));

  // and then start calling the necessary glue method
  __ee__ call(Glue::invoke, dx, builder->isolate, tx, builder->params);

  // finally do a fast test after the invocation
  __ee__ test(dx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(CALL_0_INLINE, builder, instruction) {
  // prepare the register slots we require
  auto info = Immediate(builder->info);
  auto dx = __ee__ slot(instruction->get<0>());

  // we define an empty set of parameters
  __ee__ params();

  // and then start calling the necessary glue method
  __ee__ call(Glue::recall, dx, builder->isolate, info, builder->params);

  // finally do a fast test after the invocation
  __ee__ test(dx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(CALL_N_INLINE, builder, instruction) {
  // prepare the register slots we require
  auto info = Immediate(builder->info);
  auto dx = __ee__ slot(instruction->get<0>());

  // we define a baseline set of parameters
  __ee__ params(instruction->get<1>());

  // and then start calling the necessary glue method
  __ee__ call(Glue::recall, dx, builder->isolate, info, builder->params);

  // finally do a fast test after the invocation
  __ee__ test(dx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(CALL_0_FIELD, builder, instruction) {
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
  __ee__ call(Glue::invoke, dx, builder->isolate, vx, builder->params);

  // finally do a fast test after the invocation
  __ee__ test(dx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(CALL_N_FIELD, builder, instruction) {
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
  __ee__ call(Glue::invoke, dx, builder->isolate, vx, builder->params);

  // finally do a fast test after the invocation
  __ee__ test(dx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(SPAWN_0_VOID, builder, instruction) {
  // prepare the register slots we require
  auto dx = __ee__ slot(instruction->get<0>());
  auto tx = __ee__ slot(Register::Accumulator);

  // we define an empty set of parameters
  __ee__ params();

  // and then start calling the necessary glue method
  __ee__ call(Glue::spawn, dx, builder->isolate, tx, builder->params);

  // finally do a fast test after the invocation
  __ee__ test(dx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(SPAWN_N_VOID, builder, instruction) {
  // prepare the span to be used
  auto span = instruction->get<1>();

  // prepare the register slots we require
  auto tx = __ee__ slot(span.first());
  auto dx = __ee__ slot(instruction->get<0>());

  // we define a baseline set of parameters
  __ee__ params(span.slice(1));

  // and then start calling the necessary glue method
  __ee__ call(Glue::invoke, dx, builder->isolate, tx, builder->params);

  // finally do a fast test after the invocation
  __ee__ test(dx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(SPAWN_0_FIELD, builder, instruction) {
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
  __ee__ call(Glue::spawn, dx, builder->isolate, vx, builder->params);

  // finally do a fast test after the invocation
  __ee__ test(dx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(SPAWN_N_FIELD, builder, instruction) {
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
  __ee__ call(Glue::spawn, dx, builder->isolate, vx, builder->params);

  // finally do a fast test after the invocation
  __ee__ test(dx, Validate::FAST);
}
