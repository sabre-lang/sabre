/// Syntax Includes
#include "sabre/syntax/_inline/statement.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_STMT(Break, parser) {
  auto *token = m_assert(parser->advance(), Lexer::Kind::FLOW_BREAK);
  return parser->allocate<Syntax::Break>(Syntax::Bounds(token));
}
