/// Syntax Includes
#include "sabre/syntax/_inline/expression.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_INFIX(Infer, parser, value, ) {
  // prepare the snapshot now
  auto snapshot = parser->snapshot().offset(value);

  // ensure the cast operator is actually available
  auto *token = m_assert(parser->advance(), Lexer::Kind::BINOP_IS);

  // check for any potential inversions to be used
  auto negate = parser->match(Lexer::Kind::UNOP_NOT);

  // validate the incoming guard to be used
  auto *guard = m_annotation(parser);
  if (guard == nullptr) return nullptr;

  // and construct the resulting guard now
  return parser->allocate<Syntax::Infer>(snapshot.enclose(token), value, guard, negate);
}
