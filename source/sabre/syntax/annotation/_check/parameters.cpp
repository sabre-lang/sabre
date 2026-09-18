/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Parameters, node, analyzer) { analyzer->visit(node->list()); }
SABRE_MM_CHECK_NODE(Parameters, node, analyzer) {
  // prepare the output entities to be used now
  auto entities = std::vector<Type::Entity>();
  entities.reserve(node->list().size());

  // prepare the parameters view to be used
  for (const auto &parameter : node->list()) {
    // prepare the parameter trace to be used
    $_UNUSED $_AUTO = analyzer->trace(parameter);

    // prepare the underlying entity to be updated now
    auto &entity = entities.emplace_back(Type::Entity());

    entity.modifiers() = parameter->modifiers(); // bind the initial modifiers and types for entity/parameter
    parameter->trivia()->type() = entity.value() = analyzer->check(parameter->hint(), Type::New::any()).type;

    // ignore if there is not given initializer at all
    if (parameter->initializer() == nullptr) continue;

    // get the initializer value now
    auto initializer = analyzer->check(parameter->initializer()).type;

    // update the modifiers to now include optionality
    entity.modifiers().set(Variable::Flag::OPTIONAL);

    // handle validating the required optionality now
    if (parameter->optional() && initializer->is<Type::None>()) continue;
    else if (entity.value()->unify(initializer)) continue;

    // invalidate the initializer here as needed
    analyzer->report(3000300, *initializer, *entity.value());
  }

  // and construct the resulting signature to be used
  if (node->spread()) return Type::New::variadic(Type::New::unset(), entities);
  else return Type::New::function(Type::New::unset(), entities); // non-variadic
}
