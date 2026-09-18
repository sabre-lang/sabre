/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Block, node, analyzer) {
  if (node->empty()) return; // ignore empty
  $_UNUSED $_AUTO = analyzer->scope();
  analyzer->visit(node->statements());
}

SABRE_MM_CHECK_NODE(Block, node, analyzer) {
  // ignore if there are no statements available
  if (node->empty()) return analyzer->passable();

  // otherwise attempt scoping the result now
  $_UNUSED $_AUTO = analyzer->scope();
  return analyzer->check(node->statements());
}
