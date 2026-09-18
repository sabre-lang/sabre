/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Text, , ) {}
SABRE_MM_CHECK_NODE(Text, , ) { return Type::New::string(); }
