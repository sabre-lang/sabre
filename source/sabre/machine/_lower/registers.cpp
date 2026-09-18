/// Machine Includes
#include "sabre/machine/_inline/macros.ipp"

//  EMITTER METHODS  //

SABRE_MM_MACHINE_EMIT(REG_MOVE, builder, instruction) { __ee__ move(instruction->get<0>(), instruction->get<1>()); }
SABRE_MM_MACHINE_EMIT(REG_SWAP, builder, instruction) { __ee__ swap(instruction->get<0>(), instruction->get<1>()); }
