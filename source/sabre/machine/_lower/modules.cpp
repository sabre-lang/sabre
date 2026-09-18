/// Machine Includes
#include "sabre/machine/_inline/macros.ipp"

//  EMITTER METHODS  //

SABRE_MM_MACHINE_EMIT(MODULE_OPEN, builder, instruction) {
  auto dx = __ee__ slot(instruction->get<0>());
  __ee__ call(Glue::open, dx, builder->isolate, builder->frame);
}

SABRE_MM_MACHINE_EMIT(MODULE_CLOSE, builder, instruction) {
  auto dx = __ee__ slot(instruction->get<0>());
  __ee__ call(Glue::close, dx, builder->isolate, builder->frame);
}

SABRE_MM_MACHINE_EMIT(MODULE_BARREL, builder, instruction) {
  // get the barrel register to be used
  auto tx = __ee__ slot(instruction->get<0>());

  // and attempt calling our necessary handler
  __ee__ call(Glue::barrel, tx, builder->isolate, builder->frame, tx);

  // check if we have an empty result and panic when we do
  __ee__ test(tx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(MODULE_IMPORT, builder, instruction) {
  // prepare the necessary arguments
  auto dx = __ee__ slot(instruction->get<0>());

  // prepare the intern reference to be used now
  auto index = instruction->get<1>();
  auto *arena = builder->info->arena();
  auto *intern = &arena->strings[index];

  // attempt importing into the required slot
  __ee__ call(Glue::import, dx, builder->isolate, builder->frame, Immediate(intern));

  // validate the outgoing result
  __ee__ test(dx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(MODULE_EXPORT, builder, instruction) {
  // prepare a suitable output register
  auto dx = __cc__ new_gp64();

  // prepare the necessary arguments
  auto sx = __ee__ slot(instruction->get<0>());

  // prepare the intern reference to be used now
  auto index = instruction->get<1>();
  auto *arena = builder->info->arena();
  auto *intern = &arena->strings[index];

  // attempt exporting the value now
  __ee__ call(Glue::expose, dx, builder->isolate, builder->frame, sx, Immediate(intern));

  // validate the outgoing result
  __ee__ test(dx, Validate::FAST);
}
