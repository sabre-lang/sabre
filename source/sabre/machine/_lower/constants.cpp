/// Machine Includes
#include "sabre/machine/_inline/macros.ipp"

//  EMITTER METHODS  //

SABRE_MM_MACHINE_EMIT(LOAD_SELF, builder, instruction) { __ee__ self(instruction->get<0>()); }
SABRE_MM_MACHINE_EMIT(LOAD_ZERO, builder, instruction) { __ee__ load(instruction->get<0>(), Number::Zero); }
SABRE_MM_MACHINE_EMIT(LOAD_ONE, builder, instruction) { __ee__ load(instruction->get<0>(), Number::One); }
SABRE_MM_MACHINE_EMIT(LOAD_VOID, builder, instruction) { __ee__ load(instruction->get<0>(), Constants::Void); }
SABRE_MM_MACHINE_EMIT(LOAD_TRUE, builder, instruction) { __ee__ load(instruction->get<0>(), Value::True); }
SABRE_MM_MACHINE_EMIT(LOAD_FALSE, builder, instruction) { __ee__ load(instruction->get<0>(), Value::False); }

SABRE_MM_MACHINE_EMIT(LOAD_CONST, builder, instruction) {
  auto index = instruction->get<1>();
  auto *arena = builder->info->arena();
  auto constant = arena->constants[index];
  __ee__ load(instruction->get<0>(), constant);
}

SABRE_MM_MACHINE_EMIT(LOAD_GLOBAL, builder, instruction) {
  // get some details about the load
  auto index = instruction->get<1>();
  auto *arena = builder->info->arena();

  // prepare the symbol and destination arguments
  auto sx = __ee__ ref(arena->constants[index]);
  auto dx = __ee__ slot(instruction->get<0>());

  // call the global getter with our arguments
  __ee__ call(Glue::global, dx, builder->isolate, sx);
}
