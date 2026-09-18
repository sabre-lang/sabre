/// Machine Includes
#include "sabre/machine/_inline/macros.ipp"

//  EMITTER METHODS  //

SABRE_MM_MACHINE_EMIT(UNOP_NOT, builder, instruction) { __ee__ unary(instruction); }
SABRE_MM_MACHINE_EMIT(UNOP_INV, builder, instruction) { __ee__ unary(instruction); }
SABRE_MM_MACHINE_EMIT(UNOP_NEG, builder, instruction) { __ee__ negate(instruction); }
SABRE_MM_MACHINE_EMIT(UNOP_INC, , ) { $_ABORT("Unimplemented 'Machine.lower(Operator::Kind::UNOP_INC)'"); }
SABRE_MM_MACHINE_EMIT(UNOP_DEC, , ) { $_ABORT("Unimplemented 'Machine.lower(Operator::Kind::UNOP_DEC)'"); }

SABRE_MM_MACHINE_EMIT(BINOP_ADD, builder, instruction) { __ee__ binary(instruction); }
SABRE_MM_MACHINE_EMIT(BINOP_SUB, builder, instruction) { __ee__ binary(instruction); }
SABRE_MM_MACHINE_EMIT(BINOP_MUL, builder, instruction) { __ee__ binary(instruction); }
SABRE_MM_MACHINE_EMIT(BINOP_DIV, builder, instruction) { __ee__ binary(instruction); }
SABRE_MM_MACHINE_EMIT(BINOP_MOD, builder, instruction) { __ee__ binary(instruction); }
SABRE_MM_MACHINE_EMIT(BINOP_POW, builder, instruction) { __ee__ binary(instruction); }

SABRE_MM_MACHINE_EMIT(BINOP_SHL, builder, instruction) { __ee__ binary(instruction); }
SABRE_MM_MACHINE_EMIT(BINOP_SHR, builder, instruction) { __ee__ binary(instruction); }
SABRE_MM_MACHINE_EMIT(BINOP_XOR, builder, instruction) { __ee__ binary(instruction); }
SABRE_MM_MACHINE_EMIT(BINOP_BOR, builder, instruction) { __ee__ binary(instruction); }
SABRE_MM_MACHINE_EMIT(BINOP_BAND, builder, instruction) { __ee__ binary(instruction); }

SABRE_MM_MACHINE_EMIT(BINOP_ADDI, builder, instruction) { __ee__ binary(instruction); }
SABRE_MM_MACHINE_EMIT(BINOP_SUBI, builder, instruction) { __ee__ binary(instruction); }
SABRE_MM_MACHINE_EMIT(BINOP_MULI, builder, instruction) { __ee__ binary(instruction); }
SABRE_MM_MACHINE_EMIT(BINOP_DIVI, builder, instruction) { __ee__ binary(instruction); }
SABRE_MM_MACHINE_EMIT(BINOP_MODI, builder, instruction) { __ee__ binary(instruction); }
SABRE_MM_MACHINE_EMIT(BINOP_POWI, builder, instruction) { __ee__ binary(instruction); }

SABRE_MM_MACHINE_EMIT(BINOP_SHLI, builder, instruction) { __ee__ binary(instruction); }
SABRE_MM_MACHINE_EMIT(BINOP_SHRI, builder, instruction) { __ee__ binary(instruction); }
SABRE_MM_MACHINE_EMIT(BINOP_XORI, builder, instruction) { __ee__ binary(instruction); }
SABRE_MM_MACHINE_EMIT(BINOP_BORI, builder, instruction) { __ee__ binary(instruction); }
SABRE_MM_MACHINE_EMIT(BINOP_BANDI, builder, instruction) { __ee__ binary(instruction); }

SABRE_MM_MACHINE_EMIT(BINOP_COAL, builder, instruction) { __ee__ coalesce(instruction); }
SABRE_MM_MACHINE_EMIT(BINOP_LOR, builder, instruction) { __ee__ logical(instruction); }
SABRE_MM_MACHINE_EMIT(BINOP_LAND, builder, instruction) { __ee__ logical(instruction); }

SABRE_MM_MACHINE_EMIT(TEST_EQ, builder, instruction) { __ee__ equality(instruction); }
SABRE_MM_MACHINE_EMIT(TEST_NE, builder, instruction) { __ee__ equality(instruction); }

SABRE_MM_MACHINE_EMIT(TEST_GT, builder, instruction) { __ee__ compare(instruction); }
SABRE_MM_MACHINE_EMIT(TEST_LT, builder, instruction) { __ee__ compare(instruction); }
SABRE_MM_MACHINE_EMIT(TEST_GE, builder, instruction) { __ee__ compare(instruction); }
SABRE_MM_MACHINE_EMIT(TEST_LE, builder, instruction) { __ee__ compare(instruction); }

SABRE_MM_MACHINE_EMIT(TEST_GTI, builder, instruction) { __ee__ compare(instruction); }
SABRE_MM_MACHINE_EMIT(TEST_LTI, builder, instruction) { __ee__ compare(instruction); }
SABRE_MM_MACHINE_EMIT(TEST_GEI, builder, instruction) { __ee__ compare(instruction); }
SABRE_MM_MACHINE_EMIT(TEST_LEI, builder, instruction) { __ee__ compare(instruction); }

SABRE_MM_MACHINE_EMIT(UNOP_TPL, , ) { $::System::unreachable(); }
SABRE_MM_MACHINE_EMIT(BINOP_TPL, , ) { $::System::unreachable(); }
SABRE_MM_MACHINE_EMIT(BINOP_TPLI, , ) { $::System::unreachable(); }
