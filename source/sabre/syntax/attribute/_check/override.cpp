/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Declarations
$_FWD(Sabre::Type::Dispatch, Algorithm *fields(const Erased &))
$_FWD(Sabre::Type::Dispatch, Deduction attribute(Analyzer *, const Erased &, const Syntax::Override *))

//  PUBLIC METHODS  //

Sabre::Type::Algorithm *Sabre::Type::Dispatch::fields(const Erased &type) {
  if (type->is<Instance>()) return &type->as<Instance>()->prototype()->fields();
  else return type->is<Structure>() ? &type->as<Structure>()->fields() : nullptr;
}

Sabre::Type::Deduction
Sabre::Type::Dispatch::attribute(Analyzer *analyzer, const Erased &type, const Syntax::Override *attribute) {
  // prepare some baseline functions to be used
  static Erased s_empty = New::function();

  // handle based on the incoming attribute kind
  switch (attribute->kind()) {
  // disposables expect any empty function (eg: fn -> Void).
  case Operator::Kind::DISP: {
    if (s_empty->unify(type)) return analyzer->passable(type);
    return analyzer->report(attribute->target(), 3000802, attribute->label(), *s_empty, *type);
  }

  // declare all other attempts as invalid currently
  default: return analyzer->report(3000804, *type);
  }
}

SABRE_MM_CAPTURE_NODE(Override, node, analyzer) { analyzer->visit(node->target()); }

SABRE_MM_CHECK_NODE(Override, node, analyzer) {
  // set the current trace handler
  $_UNUSED $_AUTO = analyzer->trace(node);

  // get the current preamble target to be validated
  auto *entity = analyzer->preamble();

  // we want to check the incoming "self" value
  auto self = analyzer->check(node->target());
  auto *fields = Type::Dispatch::fields(self.type);

  // check if we have a valid instance (eg: must have fields)
  if (fields == nullptr) return analyzer->report(3000803, *self.type, node->label());

  // to which we can then update the necessary lookup now
  auto valid = fields->emplace(node->symbol(), entity->value());
  if (!valid) return analyzer->report(6000303, node->label(), *self.type);

  // attempt validing the incoming operator now
  return Type::Dispatch::attribute(analyzer, entity->value(), node);
}
