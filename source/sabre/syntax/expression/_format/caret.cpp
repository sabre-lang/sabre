/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_FORMAT_PREFIX(Caret, reader) { return reader->storage()->unicode(reader->advance()->lexeme()); }
