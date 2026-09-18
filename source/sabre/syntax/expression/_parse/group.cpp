/// Syntax Includes
#include "sabre/syntax/_inline/expression.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_PREFIX(Group, parser, ) {
  // prepare the snapshot to be used now
  auto snapshot = parser->snapshot();

  // ensure the underlying token is valid
  m_assert(parser->advance(), Lexer::Kind::PUNC_LPAREN);

  // prepare an empty baseline value to be used
  Syntax::Expression *value = m_expression(parser);

  // ensure we have a value and trailing parenthesis
  if (value == nullptr || !parser->expect(Lexer::Kind::PUNC_RPAREN)) return nullptr;

  // and construct the resulting grouping now
  return parser->allocate<Syntax::Group>(snapshot.bounds(), value);
}
