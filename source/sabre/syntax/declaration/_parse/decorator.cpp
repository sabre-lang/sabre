/// Syntax Includes
#include "sabre/syntax/_inline/declaration.ipp"

//  PRIVATE METHODS  //

Sabre::Syntax::Decorator *Sabre::Parser::Dispatch::m_decorator(Stream *parser) {
  // prepare the snapshot to be used
  auto snapshot = parser->snapshot();

  // ensure there is the leading token available
  m_assert(parser->advance(), Lexer::Kind::PUNC_DECOR);

  // parse the incoming expression to be used now
  auto *expression = m_expression(parser);
  if (expression == nullptr) return nullptr;

  // should be a valid decorator to construct
  return parser->allocate<Syntax::Decorator>(snapshot.bounds(), expression);
}

SABRE_MM_PARSE_DECL(Decorator, parser) { return m_decorator(parser); }
