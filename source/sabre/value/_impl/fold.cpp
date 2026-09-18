/// Syntax Includes
#include "sabre/syntax/_inline/expression.ipp"

/// Value Includes
#include "sabre/value/_inline/value.ipp"

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Value::Fold(const Syntax::Node *node) {
  // ignore if the node is missing
  if (node == nullptr) return Value::Failure();

  // otherwise handle based on the incoming hash
  switch (node->trivia()->hash()) {
  // we have some normal constants that we can bind
  case $::RTTI::Hash<Syntax::True>(): return Value::True;
  case $::RTTI::Hash<Syntax::False>(): return Value::False;
  case $::RTTI::Hash<Syntax::Void>(): return Value::Void();

  // tagged numbers can be directly constructed as necessary
  case $::RTTI::Hash<Syntax::Numeric>(): return Number::Tagged(node->as<Syntax::Numeric>()->value());

  // groups just return a value as needed
  case $::RTTI::Hash<Syntax::Group>(): return Fold(node->as<Syntax::Group>()->value());

  // unary expressions will push further onward
  case $::RTTI::Hash<Syntax::Unary>(): {
    auto *unary = node->as<Syntax::Unary>(); // resolve
    return Fold(unary->opcode(), Fold(unary->operand()));
  }

  // binary expressions will push further onward.
  case $::RTTI::Hash<Syntax::Binary>(): {
    auto *binary = node->as<Syntax::Binary>(); // resolve
    auto *left = binary->left(), *right = binary->right();
    return Fold(binary->opcode(), Fold(left), Fold(right));
  }

  // ternaries can be resolved down their suitable paths
  case $::RTTI::Hash<Syntax::Ternary>(): {
    auto *ternary = node->as<Syntax::Ternary>();
    auto condition = Fold(ternary->condition());
    if (!condition.pointer().okay()) return Value::Failure();

    // handle based on the incoming path now
    return Value::Fold(condition.truthiness() ? ternary->condition() : ternary->alternative());
  }

  // declare that a failure has occured now
  default: return Value::Failure();
  }
}

Sabre::Value::Any Sabre::Value::Fold(Operator::Kind kind, const Value::Any &value) {
  // ignore immediately if the value is invalid
  if (!value.pointer().okay()) return value;

  // handle based on the incoming opcode given
  switch (kind) {
  case Operator::Kind::NOT: return Value::Boolean(!value.truthiness()); // truthiness is common here
  default: return value.is<Number::Tagged>() ? Fold(kind, value.as<Number::Tagged>()) : Value::Failure();
  }
}

Sabre::Value::Any Sabre::Value::Fold(Operator::Kind kind, const Number::Tagged &value) {
  switch (kind) {
  case Operator::Kind::NEG: return -value;
  case Operator::Kind::INV: return ~value;
  default: return Value::Failure();
  }
}

Sabre::Value::Any Sabre::Value::Fold(Operator::Kind kind, const Value::Any &left, const Value::Any &right) {
  // ignore immediately if the values are invalid
  if (!left.pointer().okay() || !right.pointer().okay()) return Value::Failure();

  // handle baseline opcodes as given
  switch (kind) {
  case Operator::Kind::EQ: return Value::Boolean(left == right);
  case Operator::Kind::NE: return Value::Boolean(left != right);
  default: break; // otherwise we check for valid numerics to compare
  }

  // handle if both items given are numerics
  if (!left.is<Number::Tagged>() || !right.is<Number::Tagged>()) return Value::Failure();
  else return Fold(kind, left.as<Number::Tagged>(), right.as<Number::Tagged>()); // join
}

Sabre::Value::Any Sabre::Value::Fold(Operator::Kind kind, const Number::Tagged &left, const Number::Tagged &right) {
  switch (kind) {
  // baseline numeric operators
  case Operator::Kind::ADD: return left + right;
  case Operator::Kind::SUB: return left - right;
  case Operator::Kind::MUL: return left * right;
  case Operator::Kind::DIV: return left / right;
  case Operator::Kind::MOD: return left % right;
  case Operator::Kind::POW: return left.power(right);
  case Operator::Kind::BOR: return left | right;
  case Operator::Kind::BAND: return left & right;
  case Operator::Kind::XOR: return left ^ right;
  case Operator::Kind::SHL: return left << right;
  case Operator::Kind::SHR: return left >> right;

  // baseline numeric comparisons
  case Operator::Kind::LE: return Value::Boolean(left <= right);
  case Operator::Kind::GE: return Value::Boolean(left >= right);
  case Operator::Kind::LT: return Value::Boolean(left < right);
  case Operator::Kind::GT: return Value::Boolean(left > right);

  // otherwise default to a normal output value now
  default: return Value::Failure();
  }
}
