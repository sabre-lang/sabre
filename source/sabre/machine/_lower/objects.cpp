/// Machine Includes
#include "sabre/machine/_inline/macros.ipp"

//  EMITTER METHODS  //

SABRE_MM_MACHINE_EMIT(OBJECT_EMPTY, builder, instruction) {
  auto dx = __ee__ slot(instruction->get<0>()); // prepare destination
  __ee__ call(Glue::empty<Object::Instance>, dx, builder->isolate);
}

SABRE_MM_MACHINE_EMIT(OBJECT_MAKE, builder, instruction) {
  // prepare the destination register
  auto dx = __ee__ slot(instruction->get<0>());

  // ensure we preload the necessary parameters
  __ee__ params(instruction->get<1>());

  // and invoke the enumeration constructor
  __ee__ call(Glue::object, dx, builder->isolate, builder->params);
}

SABRE_MM_MACHINE_EMIT(OBJECT_ATTR, builder, instruction) {
  // prepare the necessary slots to be used
  auto tx = __ee__ slot(instruction->get<0>());
  auto cx = __ee__ slot(instruction->get<1>());

  // resolve the internal kind value as necessary
  auto kind = static_cast<Operator::Kind>(instruction->get<2>().encode());

  // and call the required override dispatcher
  __ee__ call(Glue::overrides, cx, builder->isolate, tx, cx, kind);

  // finally validate the result value
  __ee__ test(cx, Validate::FAST);
}
