/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Placeholder, node, analyzer) {
  // bind this declaration now as a suitable symbol
  analyzer->symbol(node->name(), node, XLSP::Symbol::Kind::TPARAM);

  // get some base analyzer details
  auto *mirrors = analyzer->mirrors();
  auto *references = analyzer->references();

  // bind the suitable annotation definition
  auto *self = mirrors->resolve(node);
  references->annotate(node->name(), self);

  // and finally update all the details for the placeholder
  analyzer->verify(node->extends(), node);
  analyzer->verify(node->fallback(), node);
}
