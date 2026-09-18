/// Machine Includes
#include "sabre/machine/_inline/macros.ipp"

//  EMITTER METHODS  //

SABRE_MM_MACHINE_EMIT(CLASS_BIND, builder, instruction) {
  // prepare the function information to be encoded
  auto index = instruction->get<1>();
  auto *arena = builder->info->arena();
  auto &info = arena->functions.at(index);

  // prepare the target prototype to bind
  auto dx = __cc__ new_gp64();
  auto tx = __ee__ slot(instruction->get<0>());

  // request the binding of the constructor to the class
  __ee__ call(Glue::constructor, dx, builder->isolate, tx, Immediate(info.get()), builder->frame);

  // finally ensure that we had no errors at all
  __ee__ test(dx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(CLASS_MAKE, builder, instruction) {
  // prepare the function information to be encoded
  auto index = instruction->get<1>();
  auto *arena = builder->info->arena();
  auto *intern = &arena->strings.at(index);
  auto shape = instruction->get<2>().encode();

  // prepare the output slot to be used
  auto dx = __ee__ slot(instruction->get<0>());

  // attempt constructing the result details
  __ee__ call(Glue::prototype, dx, builder->isolate, Immediate(intern), shape, dx);
}

SABRE_MM_MACHINE_EMIT(CLASS_SUPER, builder, instruction) {
  // prepare the baseline output
  auto dx = __ee__ slot(instruction->get<0>());

  // attempt constructing the required super invocation
  __ee__ params(instruction->get<1>());

  // call the required super factory now
  __ee__ call(Glue::super, dx, builder->isolate, builder->frame, builder->params);

  // test that the result was a success
  __ee__ test(dx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(CLASS_EXPORT, builder, instruction) {
  // prepare the function information to be encoded
  auto index = instruction->get<1>();
  bool immutable = instruction->get<2>();
  auto *arena = builder->info->arena();
  auto *intern = &arena->strings.at(index);

  // prepare the registers to be used now
  auto dx = __cc__ new_gp64();
  auto tx = __ee__ slot(instruction->get<0>());

  // call the required member export now
  __ee__ call(Glue::member, dx, builder->isolate, builder->frame, Immediate(intern), tx, immutable);

  // and ensure the result is valid now
  __ee__ test(dx, Validate::FAST);
}
