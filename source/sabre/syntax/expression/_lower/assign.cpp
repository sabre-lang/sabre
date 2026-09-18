/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Assign, node, compiler, destination) {
  // handle based on the target details to be used
  switch (auto *target = node->target(); target->trivia()->hash()) {
  case $::RTTI::Hash<Syntax::Accessor>(): {
    // prepare the accessor details to be used
    auto *accessor = target->as<Syntax::Accessor>();
    auto treg = compiler->registers()->temporary();
    auto index = compiler->symbol(accessor->field()->name());

    // compile the parent node and place in the accumulator
    compiler->lower(accessor->parent(), treg);
    compiler->lower(node->value(), Register::Accumulator);

    // prepare the trace before emitting our instructions
    $_UNUSED $_AUTO = compiler->trace(accessor->parent());

    // and attempt storing our field as needed for assignment
    compiler->emit<Glyph::STORE_FIELD>(treg, Register::Accumulator, index);
    compiler->plug<Glyph::REG_MOVE>(destination, Register::Accumulator);
  } break;

  case $::RTTI::Hash<Syntax::Identifier>(): {
    // prepare the identifier details to be used
    auto *identifier = target->as<Syntax::Identifier>();
    auto extent = compiler->captures()->extent(identifier);

    // compile the incoming value to be assigned
    compiler->lower(node->value(), Register::Accumulator);

    // prepare the trace before emitting our instructions
    $_UNUSED $_AUTO = compiler->trace(identifier);

    // and attempt storing our field as needed for assignment
    compiler->store(identifier->name(), extent, Register::Accumulator);
    compiler->plug<Glyph::REG_MOVE>(destination, Register::Accumulator);
  } break;

  // otherwise declare an invalid assignment now
  default: compiler->panic(2000500); break;
  }
}
