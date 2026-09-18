/// Syntax Includes
#include "sabre/syntax/_inline/expression.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_PREFIX(Numeric, parser, ) {
  auto *token = m_assert(parser->advance(), Lexer::Flag::NUMERIC);
  return parser->allocate<Syntax::Numeric>(Syntax::Bounds(token), token);
}
