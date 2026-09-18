/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Caret, , ) {}
SABRE_MM_CHECK_NODE(Caret, node, analyzer) {
  Type::Erased callee = analyzer->world()->callee(); // validate now as needed
  return callee ? analyzer->passable(callee) : analyzer->report(node, 3000551);
}
