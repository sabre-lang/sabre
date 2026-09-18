/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_FORMAT_STMT(Break, reader, ) { return reader->storage()->unicode(reader->advance()->lexeme()); }
