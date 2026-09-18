/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Group, node, analyzer) { analyzer->visit(node->value()); }
SABRE_MM_CHECK_NODE(Group, node, analyzer) { return analyzer->check(node->value()); }
