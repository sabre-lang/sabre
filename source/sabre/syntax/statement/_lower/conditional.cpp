/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Conditional, node, compiler, ) {
  // get the base condition to be handled
  auto *condition = node->condition();

  // get the incoming truthiness value to be used
  auto truthiness = condition->trivia()->truthiness();

  // bypass everything if the condition is expected to be truthy/falsey
  if (truthiness.roughly(true)) return compiler->lower(condition), compiler->lower(node->consequence());
  if (truthiness.roughly(false)) return compiler->lower(condition), compiler->lower(node->alternative());

  auto *labels = compiler->labels(); // prepare labels
  auto skip = labels->reserve(), exit = labels->reserve();

  // prepare the incoming conditional to be used
  compiler->lower(condition, Register::Accumulator);
  compiler->emit<Glyph::JUMP_FALSEY>(skip, Register::Accumulator);

  // prepare the consequence handler now
  compiler->lower(node->consequence()), compiler->emit<Glyph::JUMP_TO>(exit);

  // prepare the alternative handler now
  labels->patch(skip), compiler->lower(node->alternative());

  // and finally patch the exit label
  labels->patch(exit);
}
