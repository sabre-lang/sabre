/// Machine Includes
#include "sabre/machine/_inline/macros.ipp"

//  EMITTER METHODS  //

SABRE_MM_MACHINE_EMIT(TYPE_CAST, builder, instruction) {
  // prepare all the registers required
  auto dx = __ee__ slot(instruction->get<0>());
  auto vx = __ee__ slot(instruction->get<1>());
  auto gx = __ee__ slot(instruction->get<2>());

  // attempt invoking the type-cast handler
  __ee__ call(Glue::ensure, dx, builder->isolate, vx, gx);

  // and fast-exit depending on the result
  __ee__ test(dx, Validate::FAST);
}

SABRE_MM_MACHINE_EMIT(TYPE_GUARD, builder, instruction) {
  // prepare all the registers required
  auto dx = __ee__ slot(instruction->get<0>());
  auto vx = __ee__ slot(instruction->get<1>());
  auto gx = __ee__ slot(instruction->get<2>());

  // prepare a label for setting a result
  auto tx = __cc__ new_gp64();
  auto bail = __cc__ new_label();

  // attempt invoking the type-cast handler
  __ee__ call(Glue::extends, tx, vx, gx);

  // set the baseline result now
  __ee__ load(dx, Value::True);

  // convert the result to a boolean now
  __cc__ j(bail, asmjit::ujit::cmp_ne(tx, Engine::Subtype::SUCCESS));

  // if we did not jump, then resolve the necessary value instead
  __ee__ load(dx, Value::False), __cc__ bind(bail);
}
