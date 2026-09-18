/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Interface, node, analyzer) {
  // we want to scope the incoming interface details
  $_UNUSED $_AUTO = analyzer->scope();

  // get some base analyzer details now
  auto *mirrors = analyzer->mirrors();
  auto *references = analyzer->references();

  // bind all the available fields now
  analyzer->verify(node->properties(), node);

  // after binding them all to the scope, we bind them to the instance
  mirrors->resolve(node)->fields() = references->view();
}
