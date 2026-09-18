/// Syntax Includes
#include "sabre/syntax/_inline/statement.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_STMT(Conditional, parser) {
  // ensure there is a leading conditional token now
  if (!parser->consume(Lexer::Kind::FLOW_IF, 2000100, "an if statement")) return nullptr;

  // prepare the baseline snapshot to be used
  auto snapshot = parser->snapshot();

  // prepare the base location to be used
  auto *token = parser->previous();

  // stop if there are no suitable parenthesis for the value
  if (!parser->expect(Lexer::Kind::PUNC_LPAREN)) return nullptr;

  // attempt parsing the condition to be used
  auto *condition = m_expression(parser);
  if (condition == nullptr) return nullptr;

  // ensure there is a closing parenthesis
  if (!parser->expect(Lexer::Kind::PUNC_RPAREN)) return nullptr;

  // attempt parsing a suitable consequence block
  auto *consequence = m_statement(parser);
  if (consequence == nullptr) return nullptr;

  auto then = parser->match(Lexer::Kind::FLOW_ELSE);
  auto *alternative = then ? m_statement(parser) : parser->allocate<Syntax::Block>();
  if (alternative == nullptr) return nullptr; // and attempt parsing an alternative

  // return the resulting conditional to be used
  return parser->allocate<Syntax::Conditional>(snapshot.enclose(token), condition, consequence, alternative);
}
