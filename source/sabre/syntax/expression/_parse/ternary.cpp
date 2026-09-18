/// Syntax Includes
#include "sabre/syntax/_inline/expression.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_INFIX(Ternary, parser, condition, ) {
  // prepare the snapshot now
  auto snapshot = parser->snapshot().offset(condition);

  // ensure there is a leading ternary to be used now
  auto *token = m_assert(parser->advance(), Lexer::Kind::PUNC_QUERY);

  // attempt parsing the consequence
  auto *consequence = m_expression(parser);

  // ensure we have a separator between our values
  if (!parser->expect(Lexer::Kind::PUNC_COLON)) return nullptr;

  // attempt parsing the alternative value now
  auto *alternative = m_expression(parser);

  // ensure our consequence and alternative are valid
  if (consequence == nullptr || alternative == nullptr) return nullptr;

  // construct the resulting ternary now
  return parser->allocate<Syntax::Ternary>(snapshot.enclose(token), condition, consequence, alternative);
}
