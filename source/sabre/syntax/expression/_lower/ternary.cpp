/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Ternary, node, compiler, destination) {
  // get the base condition to be handled
  auto *condition = node->condition();

  // get the current truthiness of the condition
  auto truthiness = condition->trivia()->truthiness();

  // bypass everything if the condition is truthy/falsey
  if (truthiness.roughly(true)) return compiler->lower(condition), compiler->lower(node->consequence(), destination);
  if (truthiness.roughly(false)) return compiler->lower(condition), compiler->lower(node->alternative(), destination);

  auto *labels = compiler->labels(); // prepare labels
  auto skip = labels->reserve(), exit = labels->reserve();

  compiler->lower(condition, Register::Accumulator); // conditional
  compiler->emit<Glyph::JUMP_FALSEY>(skip, Register::Accumulator);

  // prepare the consequence handler now
  compiler->lower(node->consequence(), destination);
  compiler->emit<Glyph::JUMP_TO>(exit); // bailout

  // prepare the alternative pathway handler
  labels->patch(skip), compiler->lower(node->alternative(), destination);

  // and finally patch the exit handler
  labels->patch(exit);
}
