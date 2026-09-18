/// Machine Includes
#include "sabre/machine/_inline/macros.ipp"

//  EMITTER METHODS  //

SABRE_MM_MACHINE_EMIT(STRING_MAKE, builder, instruction) {
  // get the intern value to be emitted
  auto index = instruction->get<1>();
  auto *arena = builder->info->arena();
  auto *intern = &arena->strings[index];

  // prepare the register to be loaded into
  auto dx = __ee__ slot(instruction->get<0>());

  // check if the intern bytes are small enough else-wise glue
  auto small = intern->bytes() < String::Limits::SMALL;
  if (small) __ee__ load(dx, String::Small(intern->view()));
  else __ee__ call(Glue::string, dx, builder->isolate, Immediate(intern));
}

SABRE_MM_MACHINE_EMIT(STRING_CONCAT, builder, instruction) {
  // prepare the required registers for concatenation
  auto dx = __ee__ slot(instruction->get<0>());
  auto ax = __ee__ slot(instruction->get<1>());
  auto bx = __ee__ slot(instruction->get<2>());

  // invoke the required glue-method
  __ee__ call(Glue::concat, dx, builder->isolate, ax, bx);
}
