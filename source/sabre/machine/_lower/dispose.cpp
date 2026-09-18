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

SABRE_MM_MACHINE_EMIT(DISPOSE_DEFER, builder, instruction) {
  auto tx = __ee__ slot(instruction->get<0>());
  __ee__ call(Glue::defer, builder->isolate, tx);
}
