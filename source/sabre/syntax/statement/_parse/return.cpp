/// Syntax Includes
#include "sabre/syntax/_inline/expression.ipp"
#include "sabre/syntax/_inline/statement.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_STMT(Return, parser) {
  auto snapshot = parser->snapshot(); // prepare the entire snapshot
  auto *token = m_assert(parser->advance(), Lexer::Kind::FLOW_RETURN);

  auto *value = parser->check(Lexer::Flag::TRAILING) ? parser->allocate<Syntax::Void>() : m_expression(parser);
  return value ? parser->allocate<Syntax::Return>(snapshot.enclose(token), value) : nullptr; // build value now
}
