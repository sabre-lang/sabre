/// Machine Includes
#include "sabre/machine/_inline/macros.ipp"

//  EMITTER METHODS  //

SABRE_MM_MACHINE_EMIT(LOAD_CONTEXT, builder, instruction) { __ee__ load(instruction); }
SABRE_MM_MACHINE_EMIT(STORE_CONTEXT, builder, instruction) { __ee__ move(instruction); }
SABRE_MM_MACHINE_EMIT(LOAD_UPVALUE, builder, instruction) { __ee__ load(instruction, instruction->get<2>()); }
SABRE_MM_MACHINE_EMIT(STORE_UPVALUE, builder, instruction) { __ee__ move(instruction, instruction->get<2>()); }
