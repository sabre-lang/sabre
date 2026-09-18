/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Placeholder, node, analyzer) {
  analyzer->visit(node->extends());
  analyzer->visit(node->fallback());
}

SABRE_MM_CHECK_NODE(Placeholder, node, analyzer) {
  // prepare the trace to be used
  $_UNUSED $_AUTO = analyzer->trace(node);

  // get some details about the placeholder
  auto name = node->name();
  auto bounds = node->trivia()->bounds();

  // resolve the extension typing to be used
  auto extends = analyzer->check(node->extends()).type;
  auto fallback = analyzer->check(node->fallback()).type;

  // ensure the fallback fulfills the incoming extension type
  if (!fallback->is<Type::Unset>() && !extends->unify(fallback)) {
    return analyzer->report(node->fallback(), 3000304, *fallback, *extends);
  }

  // construct the type-parameter to be used
  auto type = Type::New::constraint(name, extends, fallback);

  // attempt declaring into the world now
  auto failed = analyzer->world()->types().declare(name, type, bounds) == nullptr;
  return failed ? analyzer->report(4000400, name) : analyzer->passable(type);
}
