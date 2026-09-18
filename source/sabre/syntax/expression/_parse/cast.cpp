/// Syntax Includes
#include "sabre/syntax/_inline/expression.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_INFIX(Cast, parser, value, ) {
  // prepare the snapshot now
  auto snapshot = parser->snapshot().offset(value);

  // ensure the cast operator is actually available
  auto *token = m_assert(parser->advance(), Lexer::Kind::BINOP_AS);

  // validate the incoming guard to be used
  auto *guard = m_annotation(parser);
  if (guard == nullptr) return nullptr;

  // and construct the resulting guard now
  return parser->allocate<Syntax::Cast>(snapshot.enclose(token), value, guard);
}
