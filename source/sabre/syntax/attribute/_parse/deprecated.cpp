/// Syntax Includes
#include "sabre/syntax/_inline/declaration.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_DECL(Deprecated, parser) {
  // prepare the incoming snapshot
  auto snapshot = parser->snapshot();

  // get the original token location to be used
  auto *token = m_assert(parser->advance(), "Deprecated");

  // stop early if necessary to do so
  if (!parser->match(Lexer::Kind::LTRL_STR)) return parser->allocate<Syntax::Deprecated>(Syntax::Bounds(token));

  // otherwise we can safely construct with a message found
  return parser->allocate<Syntax::Deprecated>(snapshot.enclose(token), parser->previous()->lexeme());
}
