/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Numeric, , ) {}
SABRE_MM_CHECK_NODE(Numeric, , ) { return Type::New::number(); }
