/// Sabre Includes
#include "sabre/format/dispatch.hpp"

//  PRIVATE METHODS  //

bool Sabre::Format::Delimited::m_condition(Reader *) { return false; }

Sabre::Format::Piece *Sabre::Format::Delimited::m_callback(Reader *reader) { return Dispatch::m_declaration(reader); }

Sabre::Format::Delimited::Condition Sabre::Format::Delimited::m_closing(Lexer::Kind closing) {
  return [closing](Reader *reader) { return reader->check(closing); };
}