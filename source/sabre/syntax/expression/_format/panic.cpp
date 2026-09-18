/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PUBLIC METHODS  //

SABRE_MM_FORMAT_PREFIX(Panic, reader) { return m_statement<Syntax::Return>(reader); }
