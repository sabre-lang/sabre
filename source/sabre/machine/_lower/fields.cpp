/// Machine Includes
#include "sabre/machine/_inline/macros.ipp"

//  EMITTER METHODS  //

SABRE_MM_MACHINE_EMIT(LOAD_FIELD, builder, instruction) {
  // prepare the arena details to be used
  auto index = instruction->get<2>();

  // prepare all the necessary getter arguments
  auto dx = __ee__ slot(instruction->get<0>());
  auto tx = __ee__ slot(instruction->get<1>());

  // attempt executing the getter now
  __ee__ getter(dx, tx, index);
}

SABRE_MM_MACHINE_EMIT(STORE_FIELD, builder, instruction) {
  // prepare the arena details to be used
  auto index = instruction->get<2>();

  // prepare all the necessary getter arguments
  auto tx = __ee__ slot(instruction->get<0>());
  auto vx = __ee__ slot(instruction->get<1>());

  // attempt executing the getter now
  __ee__ setter(tx, vx, index);
}
