/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(For, node, analyzer) {
  $_UNUSED $_AUTO = analyzer->scope();
  analyzer->visit(node->iterable());
  analyzer->bind(node->parameters());
  analyzer->visit(node->statement());
}

SABRE_MM_CHECK_NODE(For, node, analyzer) {
  // prepare the underlying trace
  $_UNUSED $_AUTO = analyzer->trace(node);

  // get the initial condition details
  auto iterable = analyzer->check(node->iterable()).type;
  auto value = iterable->apply(Operator::Kind::ITER);

  // ignore if not actually iterable to begin with
  if (value->is<Type::Unset>()) analyzer->report(node->iterable(), 3000700, *iterable), value = Type::New::any();

  // prepare the for-loop arguments that are available
  auto arguments = std::vector<Type::Erased>({value, Type::New::number()});

  // and prepare the fixed maximum to be used
  auto maximum = arguments.size();

  // prepare the scope to be used
  auto world = analyzer->scope();
  auto values = world->values();
  world->loops() = world->depth();

  // iterate over the available parameters now
  for (const auto &[ii, parameter] : $::Ranges::Each(node->parameters())) {
    // get the current exceeding details to be used
    auto exceeds = ii >= maximum;

    // prepare some details about the parameter
    auto name = parameter->name();
    auto bounds = parameter->trivia()->bounds();

    // if the value exeeds then handle as necessary
    if (exceeds) analyzer->report(parameter, 3000701, maximum, ii);
    auto argument = exceeds ? Type::New::never() : arguments.at(ii);

    // also do not forget to assign the argument name as well
    parameter->trivia()->type() = argument;

    // ensure we can actually declare the entity instance now
    if (values.declare(name, argument, bounds) == nullptr) analyzer->report(parameter, 4000401, name);
  }

  // and return the resulting block details now
  return analyzer->check(node->statement());
}

SABRE_MM_CAPTURE_NODE(Loop, node, analyzer) {
  $_UNUSED $_AUTO = analyzer->scope();
  analyzer->visit(node->condition());
  analyzer->visit(node->statement());
}

SABRE_MM_CHECK_NODE(Loop, node, analyzer) {
  // prepare the deduction of the underlying condition
  auto deduction = analyzer->check(node->condition());
  if (deduction.type->is<Type::Any>()) return deduction;

  // determine some truths about the deduction result
  auto truthiness = deduction.type->truthiness();
  auto forever = truthiness.roughly(true);

  // show when the condition is always truthy
  if (forever) analyzer->report(node->condition(), 3000601, "run forever", true);
  else if (truthiness.roughly(false)) {
    analyzer->report(node->condition(), 3000601, "never run", false);
    return analyzer->redundant(node->statement()); // unreachable here
  }

  // attempt getting the current block details now
  auto world = analyzer->scope();
  world->loops() = world->depth();

  // attempt parsing the incoming branch as necessary
  auto left = [&](auto *_) { return _->check(node->statement()).flow; };
  auto right = [&](auto *_) { return _->passable().flow; };
  auto result = analyzer->branch(std::move(deduction), left, right);

  // handle our control flow-effect now
  switch (result.flow->effect()) {
  case Flow::Effect::BREAKS: return analyzer->passable();
  default: return forever ? analyzer->unreachable() : analyzer->passable();
  }
}
