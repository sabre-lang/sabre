/// Syntax Includes
#include "sabre/syntax/_inline/expression.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_PREFIX(True, parser, ) {
  auto *token = m_assert(parser->advance(), Lexer::Kind::LTRL_TRUE);
  return parser->allocate<Syntax::True>(Syntax::Bounds(token));
}

SABRE_MM_PARSE_PREFIX(False, parser, ) {
  auto *token = m_assert(parser->advance(), Lexer::Kind::LTRL_FALSE);
  return parser->allocate<Syntax::False>(Syntax::Bounds(token));
}
