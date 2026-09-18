/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Namespace, node, analyzer) {
  // bind this declaration now as a suitable symbol
  $_UNUSED $_AUTO = analyzer->symbol(node, XLSP::Symbol::Kind::NAMESPACE);

  // get some base analyzer details
  auto *mirrors = analyzer->mirrors();
  auto *references = analyzer->references();

  // bind this mirror as a suitable definition
  auto *block = node->block();
  auto *self = mirrors->resolve(node);

  // we allow overriding class definitions if our declaration fails
  if (!references->declare(node->name(), self)) {
    auto definition = references->resolve(node->name());
    auto *prototype = definition->variable->filter<Syntax::Class>();
    if (prototype) self->definition()->variable = self, definition->variable = self;
  }

  // prepare a custom scoping for the namespace
  $_UNUSED $_AUTO = analyzer->scope();
  references = analyzer->references();

  // we bind the incoming statements a little differently
  analyzer->verify(block, node, false);

  // iterate over the statements to bind them to the scope (even non-exported items)
  for (const auto *statement : block->statements()) analyzer->verify(statement, block);

  // we should suitably bind each of the properties (even if not-exported)
  self->fields() = references->view();
}
