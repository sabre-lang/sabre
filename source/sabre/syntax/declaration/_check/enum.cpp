/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Declarations
$_FWD(Sabre::Type::Dispatch, Erased check(const Syntax::Expression *, Analyzer *, const Erased &, Diagnostic::Code))

//  PUBLIC METHODS  //

Sabre::Type::Erased Sabre::Type::Dispatch::check(
    const Syntax::Expression *expression, Analyzer *analyzer, const Erased &expected, Diagnostic::Code code
) {
  // always default if the expression is missing
  if (expression == nullptr) return expected;

  // otherwise attempt getting the inferred typing
  auto inferred = analyzer->check(expression).type;

  // check that our inferred type is assignable to the expression
  if (expected->unify(inferred)) return expected;

  // otherwise report and return base details
  return analyzer->report(expression, code, *inferred, *expected), expected;
}

SABRE_MM_CAPTURE_NODE(Variant, node, analyzer) { analyzer->visit(node->label()), analyzer->visit(node->value()); }
SABRE_MM_CHECK_NODE(Variant, node, analyzer) {
  // trace this node now
  $_UNUSED $_AUTO = analyzer->trace(node);

  // check the label if we have been given one at all
  Type::Dispatch::check(node->label(), analyzer, Type::New::string(), 3000303);

  // otherwise the core check is the incoming
  return Type::Dispatch::check(node->value(), analyzer, Type::New::number(), 3000302);
}

SABRE_MM_CAPTURE_NODE(Enum, node, analyzer) { analyzer->bind(node), analyzer->visit(node->variants()); }
SABRE_MM_CHECK_NODE(Enum, node, analyzer) {
  // start tracing this now now
  $_UNUSED $_AUTO = analyzer->trace(node);

  // prepare some underlying details about the node
  auto name = node->name();

  // prepare the underlying variants that have been found
  auto variants = $::Map::Record<Type::Entity>();

  // assign a new shape instance now
  auto shape = analyzer->shapes()->assign(node);

  // prepare the enumeration value
  auto enumeration = Type::New::enumeration(name, shape);

  // iterate over the available enumeration choices
  for (const auto &variant : node->variants()) {
    auto emplaced = variants.try_emplace(variant->key(), enumeration).second; // emplace now
    emplaced ? analyzer->check(variant) : analyzer->report(variant, 4000405, variant->key());
  }

  // prepare a custom accessor to be used now
  auto accessor = Type::New::interface(name, variants);
  auto *entity = analyzer->world()->values().declare(analyzer->sanity(node), accessor);

  if (entity == nullptr) return analyzer->report(4000403, name);   // already exists so fail
  if (entity->transient()) return analyzer->report(4000402, name); // only exists in type-land

  // update the underlying type to be used now
  return analyzer->passable(entity->type() = enumeration);
}
