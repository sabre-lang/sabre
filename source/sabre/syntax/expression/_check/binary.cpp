/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Binary, node, analyzer) {
  analyzer->visit(node->left());
  analyzer->visit(node->right());
}

SABRE_MM_CHECK_NODE(Binary, node, analyzer) {
  // trace from the unary node
  $_UNUSED $_AUTO = analyzer->trace(node);

  // prepare the sides that are available
  auto left = analyzer->check(node->left()).type;
  auto right = analyzer->check(node->right()).type;

  // prepare the base deduction to be used
  auto deduction = analyzer->passable(Type::New::fail());

  // if either operand is invalid, then fail immediately
  if (left->is<Type::Poison>() || right->is<Type::Poison>()) return deduction;

  // otherwise attempt running the transformation now
  deduction.type = left->apply(node->opcode(), right);

  if (!deduction.type->is<Type::Unset>()) return deduction;
  return analyzer->report(3000801, node->symbol(), *left, *right);
}
