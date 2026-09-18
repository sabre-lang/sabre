/// Sabre Includes
#include "sabre/parser/delimited.hpp"

/// Syntax Includes
#include "sabre/syntax/_inline/expression.ipp"
#include "sabre/syntax/_inline/statement.ipp"

//  PRIVATE METHODS  //

Sabre::Syntax::Expression *Sabre::Parser::Dispatch::m_guard(Stream *parser) {
  if (!parser->match(Lexer::Kind::BINOP_MUL)) return m_expression(parser);
  return parser->allocate<Syntax::Fallback>(Syntax::Bounds(parser->previous()));
}

SABRE_MM_PARSE_STMT(Match, parser) {
  // prepare the baseline details now
  auto snapshot = parser->snapshot();

  // prepare the baseline token to be matched against
  auto *token = m_assert(parser->advance(), Lexer::Kind::FLOW_MATCH);

  // ensure we have a valid loop token to begin with
  if (!parser->expect(Lexer::Kind::PUNC_LPAREN)) return nullptr;

  // attempt parsing the value to be used now
  auto *value = m_expression(parser);
  if (value == nullptr) return nullptr;

  // ensure there is a closing parenthesis now
  if (!parser->expect(Lexer::Kind::PUNC_RPAREN)) return nullptr;

  // prepare the enclosed section now to be used
  static constexpr auto s_opening = Lexer::Kind::PUNC_LBRACE;
  static constexpr auto s_closing = Lexer::Kind::PUNC_RBRACE;
  static constexpr auto s_callback = m_statement<Syntax::Query>;
  static auto s_enclosed = Enclosed<Syntax::Query, s_opening, s_closing>(s_callback);

  // attempt parsing all the incoming details now
  auto queries = s_enclosed.dispatch(parser);
  if (!queries.has_value()) return nullptr;

  // finally resolve the resulting match statement now
  return parser->allocate<Syntax::Match>(snapshot.enclose(token), value, *queries);
}

SABRE_MM_PARSE_STMT(Query, parser) {
  // prepare the callback and delimited handler to be used now
  static constexpr auto s_opening = Lexer::Kind::MISC_MAX;
  static constexpr auto s_closing = Lexer::Kind::PUNC_COLON;
  static constexpr auto s_callback = [](auto *_) { return m_guard(_); };
  static auto s_enclosed = Enclosed<Syntax::Expression, s_opening, s_closing>(s_callback);

  // prepare the baseline details now
  auto snapshot = parser->snapshot();

  // attempt parsing as many expressions as possible
  auto guards = s_enclosed.dispatch(parser);
  if (!guards.has_value()) return nullptr;

  // attempt parsing an incoming statement to be used
  auto *statement = m_statement(parser, Syntax::Unterminated());

  // handle the result to be returned now
  return statement ? parser->allocate<Syntax::Query>(snapshot.bounds(), statement, *guards) : nullptr;
}
