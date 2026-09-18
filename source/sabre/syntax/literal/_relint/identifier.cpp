/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Self, node, analyzer) {
  auto *self = analyzer->mirrors()->resolve(node);
  analyzer->references()->capture(node->name(), self);
}

SABRE_MM_LINT_NODE(Identifier, node, analyzer) {
  auto *self = analyzer->mirrors()->resolve(node);
  analyzer->references()->capture(node->name(), self);
}
