/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Unary, node, analyzer) { analyzer->visit(node->operand()); }
SABRE_MM_CHECK_NODE(Unary, node, analyzer) {
  // trace from the binary node
  $_UNUSED $_AUTO = analyzer->trace(node);

  // prepare the sides that are available
  auto operand = analyzer->check(node->operand()).type;

  // prepare the base deduction to be used
  auto deduction = analyzer->passable(Type::New::fail());

  // if the operand is invalid, then fail immediately
  if (operand->is<Type::Poison>()) return deduction;

  // otherwise attempt running the transformation now
  deduction.type = operand->apply(node->opcode());

  if (!deduction.type->is<Type::Unset>()) return deduction;
  return analyzer->report(3000800, node->symbol(), *operand);
}
