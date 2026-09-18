/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Alias, node, analyzer) {
  // bind this declaration now as a suitable symbol
  $_UNUSED $_AUTO = analyzer->symbol(node, XLSP::Symbol::Kind::INTERFACE);

  // get some base analyzer details
  auto *mirrors = analyzer->mirrors();
  auto *references = analyzer->references();

  // bind the suitable annotation definition
  auto *self = mirrors->resolve(node);
  references->annotate(node->name(), self);

  // and finally update all the details for the alias
  analyzer->verify(node->hint(), node);
  analyzer->verify(node->generics(), node);
}
