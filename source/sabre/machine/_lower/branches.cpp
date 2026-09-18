/// Machine Includes
#include "sabre/machine/_inline/macros.ipp"

//  EMITTTER METHODS  //

SABRE_MM_MACHINE_EMIT(JUMP_TPL, , ) { $::System::unreachable(); }

SABRE_MM_MACHINE_EMIT(JUMP_TO, builder, instruction) {
  __ee__ irq(Engine::Interrupt::BAILOUT); // jtest
  __cc__ j(__ee__ label(instruction->get<0>()));
}

SABRE_MM_MACHINE_EMIT(JUMP_FILLED, builder, instruction) {
  // prepare some details for the incoming jump
  auto tx = __cc__ new_gp64();
  auto hole = __ee__ imm(Constants::Void);
  auto label = __ee__ label(instruction->get<0>());

  // test for incoming interupts before jump
  __ee__ irq(Engine::Interrupt::BAILOUT);

  // we want to test our value against a "Void" value
  __ee__ move(tx, instruction->get<1>());
  __cc__ j(label, asmjit::ujit::cmp_ne(tx, hole));
}

SABRE_MM_MACHINE_EMIT(JUMP_TRUTHY, builder, instruction) {
  // get the label required for jumping
  auto label = __ee__ label(instruction->get<0>());

  // test our interrupt mode then execute our jump
  __ee__ irq(Engine::Interrupt::BAILOUT);
  __ee__ jmpt(label, instruction->get<1>());
}

SABRE_MM_MACHINE_EMIT(JUMP_FALSEY, builder, instruction) {
  // get the label required for jumping
  auto label = __ee__ label(instruction->get<0>());

  // test our interrupt mode then execute our jump
  __ee__ irq(Engine::Interrupt::BAILOUT);
  __ee__ jmpf(label, instruction->get<1>());
}

SABRE_MM_MACHINE_EMIT(MATCH_VOID, builder, instruction) {
  // get the label required for jumping
  auto label = __ee__ label(instruction->get<0>());

  // test our interrupt mode before emitting the jump
  __ee__ irq(Engine::Interrupt::BAILOUT);
  __ee__ jmpc(label, instruction->get<1>(), Constants::Void);
}

SABRE_MM_MACHINE_EMIT(MATCH_TRUE, builder, instruction) {
  // get the label required for jumping
  auto label = __ee__ label(instruction->get<0>());

  // test our interrupt mode before emitting the jump
  __ee__ irq(Engine::Interrupt::BAILOUT);
  __ee__ jmpc(label, instruction->get<1>(), Value::True);
}

SABRE_MM_MACHINE_EMIT(MATCH_FALSE, builder, instruction) {
  // get the label required for jumping
  auto label = __ee__ label(instruction->get<0>());

  // test our interrupt mode before emitting the jump
  __ee__ irq(Engine::Interrupt::BAILOUT);
  __ee__ jmpc(label, instruction->get<1>(), Value::False);
}

SABRE_MM_MACHINE_EMIT(MATCH_CONST, builder, instruction) {
  // get the label required for jumping
  auto label = __ee__ label(instruction->get<0>());

  // test our interrupt mode before emitting the jump
  __ee__ irq(Engine::Interrupt::BAILOUT);
  __ee__ jmpc(label, instruction->get<1>(), instruction->get<2>());
}

SABRE_MM_MACHINE_EMIT(MATCH_TEXT, builder, instruction) {
  // get the intern value to be emitted
  auto index = instruction->get<2>();
  auto *arena = builder->info->arena();
  auto *intern = &arena->strings[index];

  // get the label required for jumping
  auto label = __ee__ label(instruction->get<0>());

  // prepare all our registers to be used
  auto dx = __cc__ new_gp64();
  auto tx = __ee__ slot(instruction->get<1>());

  // test our interrupt mode before emitting the jump
  __ee__ irq(Engine::Interrupt::BAILOUT);
  __ee__ call(Glue::compare, dx, tx, Immediate(intern));
  __cc__ j(label, asmjit::ujit::test_nz(dx));
}

SABRE_MM_MACHINE_EMIT(MATCH_GUARD, builder, instruction) {
  // get the label required for jumping
  auto label = __ee__ label(instruction->get<0>());

  // prepare all our registers to be used
  auto dx = __cc__ new_gp64();
  auto tx = __ee__ slot(instruction->get<1>());
  auto gx = __ee__ slot(instruction->get<2>());

  // test our interrupt mode before emitting the jump
  __ee__ irq(Engine::Interrupt::BAILOUT);
  __ee__ call(Glue::matches, dx, tx, gx);
  __cc__ j(label, asmjit::ujit::test_nz(dx));
}
