/// Syntax Includes
#include "sabre/syntax/_inline/declaration.ipp"

//  PRIVATE METHODS  //

Sabre::Reflect::Category Sabre::Parser::Dispatch::m_category(Stream *parser) {
  if (!parser->check(Lexer::Flag::ANNOTATES)) return Reflect::Category::INVALID;
  else return Reflect::Inspect::category(parser->current()->lexeme());
}

Sabre::Syntax::Attribute *Sabre::Parser::Dispatch::m_attribute(Stream *parser) {
  // ensure there is the leading token available
  m_assert(parser->advance(), Lexer::Kind::PUNC_ATTR);

  // ensure there is a leading bracket as well
  if (!parser->expect(Lexer::Kind::PUNC_LBRACK)) return nullptr;

  // attempt parsing an incoming category to be used
  Syntax::Attribute *attribute = nullptr;

// attempt parsing based on the incoming category now
#define X(C, N, ...)                                                              \
  case Reflect::Category::C: attribute = m_declaration<Syntax::N>(parser); break;
  switch (m_category(parser)) {
    SABRE_XX_REFLECTION_CATEGORIES(X) // prepare the baseline categories
  default: return parser->report(2000800, parser->current()->lexeme());
  }
#undef X

  // attempt checking for a closing bracket now
  if (!parser->expect(Lexer::Kind::PUNC_RBRACK)) return nullptr;

  // return the resulting attribute now
  return attribute;
}

SABRE_MM_PARSE_DECL(Attribute, parser) { return m_attribute(parser); }
