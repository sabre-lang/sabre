/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Overload, node, analyzer) { analyzer->visit(node->signature()); }

SABRE_MM_CHECK_NODE(Overload, node, analyzer) {
  return analyzer->report(node, 9000002, "Analyzer.check(Syntax::Overload)");
}
