/// Syntax Includes
#include "sabre/syntax/_inline/annotation.ipp"

//  PRIVATE METHODS  //

Sabre::Syntax::Annotation *Sabre::Parser::Dispatch::m_annotation(Stream *parser) {
  // resolve the current token to parse
  auto current = parser->current();

  // initially check against any of the potential "qualifier" variants
  if (current->flags().test(Lexer::Flag::ANNOTATES)) return m_annotation<Syntax::Qualifier>(parser);

  // handle a varienty of annotation types
  switch (current->kind()) {
  // handle special cases that could be possible
  case Lexer::Kind::DECL_FUNC: return m_annotation<Syntax::Signature>(parser);
  case Lexer::Kind::PUNC_LBRACE: return m_annotation<Syntax::Interface>(parser);

  // all other tokens should result in an immediate failure
  default: return parser->report(current, 2000100, "a type-annotation");
  }
}
