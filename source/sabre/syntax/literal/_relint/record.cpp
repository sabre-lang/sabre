/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Record, node, analyzer) {
  // we scope the incoming elements for declaration
  $_UNUSED $_AUTO = analyzer->scope();

  // get some base analyzer details now
  auto *mirrors = analyzer->mirrors();
  auto *references = analyzer->references();

  // declare the available elements to be bound
  analyzer->verify(node->elements(), node);

  // after binding them all to the scope, we bind them to the instance
  mirrors->resolve(node)->fields() = references->view();
}
