/// Syntax Includes
#include "sabre/syntax/_inline/expression.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_PREFIX(Panic, parser, ) {
  // prepare the snapshot now
  auto snapshot = parser->snapshot();

  // ensure the underlying token is valid
  auto *token = m_assert(parser->advance(), Lexer::Kind::FLOW_PANIC);

  // since we allow empty "panic" expressions, stop early when needed
  auto *value = parser->check(Lexer::Flag::TRAILING) ? parser->allocate<Syntax::Void>() : m_expression(parser);

  // attempt resolving a suitable panic expression now
  return value ? parser->allocate<Syntax::Panic>(snapshot.enclose(token), value) : nullptr;
}
