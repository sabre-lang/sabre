/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Accessor, node, analyzer) {
  // prepare the metadata and references
  auto *mirrors = analyzer->mirrors();
  auto *references = analyzer->references();

  // we attempt resolving the parent and the field details
  auto *parent = analyzer->verify(node->parent(), node);
  auto *field = analyzer->verify(node->field(), node, false);

  // we actually want to find the parent variable definition
  if (auto *variable = parent->definition()->variable) {
    auto definition = variable->lookup(node->field()->name());
    references->relate(field, definition); // link accessor
    references->relate(mirrors->resolve(node), definition);
  }
}
