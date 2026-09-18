/// Syntax Includes
#include "sabre/syntax/_inline/expression.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_PREFIX(Caret, parser, ) {
  auto *token = m_assert(parser->advance(), Lexer::Kind::BINOP_XOR);
  return parser->allocate<Syntax::Caret>(Syntax::Bounds(token));
}
