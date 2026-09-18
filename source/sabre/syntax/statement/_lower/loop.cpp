/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Loop, node, compiler, ) {
  // get the underlying condition details
  auto *condition = node->condition();
  auto truthiness = condition->trivia()->truthiness();

  // ignore if the loop never runs at all
  if (truthiness.roughly(false)) return compiler->lower(condition);

  // prepare the loop instance to be used now
  auto loop = compiler->loop();

  // compile the incoming condition onto the accumulator
  compiler->lower(condition, Register::Accumulator);

  // allow exiting if we have a non-forever loop
  if (!truthiness.roughly(true)) compiler->emit<Glyph::JUMP_FALSEY>(loop.exit(), Register::Accumulator);

  // and lower the entire body now
  compiler->lower(node->statement());
}

SABRE_MM_LOWER_NODE(For, node, compiler, ) {
  // get some common items to be used now
  auto parameters = node->parameters();

  // prepare a new variable scoping to be used
  $_UNUSED $_AUTO = compiler->scope();
  $_UNUSED $_AUTO = compiler->trace(node);

  // prepare our necessary registers to be used
  auto value = parameters.size() > 0 ? compiler->declare(parameters.at(0)).first : Register::Slot();
  auto index = parameters.size() > 1 ? compiler->declare(parameters.at(1)).first : Register::Slot();

  // prepare a suitable "temporary" for the iterator register
  auto iterator = compiler->registers()->allocate();

  // ensure we compiler our iterator into the necessary location
  $_PP_SCOPE() {
    $_UNUSED $_AUTO = compiler->trace(node->iterable());
    compiler->lower(node->iterable(), iterator); // prepare
    compiler->emit<Glyph::ITER_LOAD>(iterator, iterator);
  }

  // start the loop sequence now to be used
  auto loop = compiler->loop();

  // get the next iterator value now
  compiler->emit<Glyph::ITER_NEXT>(iterator, value, index);
  compiler->emit<Glyph::JUMP_TRUTHY>(loop.exit(), Register::Accumulator);

  // ensure we now compile our desired loop statement
  compiler->lower(node->statement());
}
