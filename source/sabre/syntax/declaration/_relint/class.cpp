/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Header, node, analyzer) {
  analyzer->verify(node->constructor(), node);
  analyzer->verify(node->super(), node);
  analyzer->verify(node->implements(), node);
}

SABRE_MM_LINT_NODE(Class, node, analyzer) {
  // bind this declaration now as a suitable symbol
  $_UNUSED $_AUTO = analyzer->symbol(node, XLSP::Symbol::Kind::CLASS);

  // get some base analyzer details
  auto *mirrors = analyzer->mirrors();
  auto *references = analyzer->references();

  // get the mirror instance to be used
  auto *self = mirrors->resolve(node);

  // bind both the value and type definitions
  references->declare(node->name(), self);
  references->annotate(node->name(), self);

  // prepare some scoping for classes to be used
  $_UNUSED $_AUTO = analyzer->scope();

  // bind the incoming details for the class
  analyzer->verify(node->header(), node);
  analyzer->verify(node->fields(), node);

  // finally bind all the available fields now (regardless of visibility)
  self->fields() = references->view();
}
