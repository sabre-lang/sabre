/// Sabre Includes
#include "sabre/parser/delimited.hpp"

/// Syntax Includes
#include "sabre/syntax/_inline/expression.ipp"
#include "sabre/syntax/_inline/statement.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_STMT(Loop, parser) {
  // prepare the baseline details now
  auto snapshot = parser->snapshot();

  // ensure we have a valid "loop" token
  auto *token = m_assert(parser->advance(), Lexer::Kind::LOOP_ANY);

  // get the reference range and the forever details
  auto forever = !parser->match(Lexer::Kind::PUNC_LPAREN);

  // parse the condition to now be used
  auto *condition = forever ? parser->allocate<Syntax::True>() : m_expression(parser);
  if (!forever && (condition == nullptr || !parser->expect(Lexer::Kind::PUNC_RPAREN))) return nullptr;

  // attempt parsing the statement for the loop
  auto *statement = m_statement(parser);
  if (statement == nullptr) return nullptr;

  // construct the resulting loop instance
  return parser->allocate<Syntax::Loop>(snapshot.enclose(token), condition, statement);
}

SABRE_MM_PARSE_STMT(For, parser) {
  // prepare the baseline details now
  auto snapshot = parser->snapshot();

  // get the base location and the iterable to be used
  auto *token = m_assert(parser->advance(), Lexer::Kind::LOOP_FOR);

  // ensure we have a valid loop token to begin with
  if (!parser->expect(Lexer::Kind::PUNC_LPAREN)) return nullptr;

  // prepare the details for parsing the parameters to be used
  static constexpr auto s_callback = [](auto *_) { return m_identifier(_); };
  static auto s_delimited = Delimited<Syntax::Identifier>(s_callback);

  // attempt parsing the parameters
  auto parameters = s_delimited.dispatch(parser);
  if (!parameters.has_value()) return nullptr;

  // ensure we have a set of iterable values
  if (!parser->expect(Lexer::Kind::BINOP_IN, "iterable")) return nullptr;

  auto *iterable = m_expression(parser);
  if (iterable == nullptr) return nullptr;

  // ensure there is a closing parenthesis now
  if (!parser->expect(Lexer::Kind::PUNC_RPAREN)) return nullptr;

  // attempt parsing the incoming for statement
  auto *statement = m_statement(parser);
  if (statement == nullptr) return nullptr;

  // and construct the resulting instance now
  return parser->allocate<Syntax::For>(snapshot.enclose(token), iterable, *parameters, statement);
}
