/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PUBLIC METHODS  //

SABRE_MM_FORMAT_INFIX(Typed, reader, value) { return m_specialize(reader, value); }
