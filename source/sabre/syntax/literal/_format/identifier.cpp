/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

Sabre::Format::Piece *Sabre::Format::Dispatch::m_identifier(Reader *reader) {
  if (!reader->match(Lexer::Flag::ANNOTATES)) return nullptr;
  return reader->storage()->unicode(reader->previous()->lexeme());
}

SABRE_MM_FORMAT_PREFIX(Self, reader) { return m_assignment(reader, m_identifier(reader)); }
SABRE_MM_FORMAT_PREFIX(Identifier, reader) { return m_assignment(reader, m_identifier(reader)); }
