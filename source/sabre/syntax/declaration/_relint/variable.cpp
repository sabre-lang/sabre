/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Variable, node, analyzer) {
  // bind this declaration now as a suitable symbol
  $_UNUSED $_AUTO = analyzer->symbol(node, XLSP::Symbol::Kind::VARIABLE);

  // get some base analyzer details
  auto *mirrors = analyzer->mirrors();
  auto *references = analyzer->references();

  // bind this mirror as a suitable definition
  auto *self = mirrors->resolve(node);
  references->declare(node->name(), self);

  // bind the incoming details for the variable
  analyzer->verify(node->hint(), node);
  analyzer->verify(node->initializer(), node);
}
