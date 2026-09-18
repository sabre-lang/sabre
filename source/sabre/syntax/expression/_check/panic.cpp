/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Panic, node, analyzer) { analyzer->visit(node->value()); }
SABRE_MM_CHECK_NODE(Panic, node, analyzer) {
  // push the necessary traces for analyzing
  $_UNUSED $_AUTO = analyzer->trace(node);

  // forcibly check the incoming value to be used now
  analyzer->check(node->value());

  // and declare as currently reachable now
  return analyzer->unreachable();
}
