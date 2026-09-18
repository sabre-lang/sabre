/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(True, , ) {}
SABRE_MM_CAPTURE_NODE(False, , ) {}

SABRE_MM_CHECK_NODE(True, , ) { return Type::New::boolean(); }
SABRE_MM_CHECK_NODE(False, , ) { return Type::New::boolean(); }
