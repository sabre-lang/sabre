/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Continue, , ) {}
SABRE_MM_CHECK_NODE(Continue, node, analyzer) {
  Flow::Degree depth = analyzer->world()->loops();
  if (depth < 0) return analyzer->report(node, 2000601, "continue");
  return analyzer->unreachable(depth, Flow::Effect::LOOPS);
}
