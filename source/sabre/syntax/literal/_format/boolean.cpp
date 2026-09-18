/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_FORMAT_PREFIX(True, reader) { return reader->storage()->unicode(reader->advance()->lexeme()); }
SABRE_MM_FORMAT_PREFIX(False, reader) { return reader->storage()->unicode(reader->advance()->lexeme()); }
