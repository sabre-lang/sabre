/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Record, node, analyzer) {
  for (const auto &element : node->elements()) {
    analyzer->visit(element->hint());
    analyzer->visit(element->initializer());
  }
}

SABRE_MM_CHECK_NODE(Record, node, analyzer) {
  // get the underlying record elements
  const auto &elements = node->elements();

  // stop early if they are empty
  if (elements.empty()) return Type::New::interface();

  // begin tracing for errors here now
  $_UNUSED $_AUTO = analyzer->trace(node);

  // prepare the fields to be bound now
  auto fields = $::Map::Record<Type::Entity>();

  // convert all our values as necessary now
  for (const auto *variable : elements) {
    // construct the incoming expected entity now
    auto [name, expected] = analyzer->vardef(variable);

    // rebuild the entity to be used now
    auto entity = Type::Entity(expected);

    // update some values that are required
    entity.modifiers() = variable->modifiers();
    variable->trivia()->type() = entity.value();

    // ensure incoming variables are not disposable
    if (variable->disposable()) analyzer->report(variable, 3000151, name);

    if (!fields.contains(name)) fields.emplace(name, entity);
    else analyzer->report(variable, 4000404, name); // failure
  }

  // construct the resulting record to be used
  return Type::New::interface(fields);
}
