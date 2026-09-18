/// Syntax Includes
#include "sabre/syntax/_inline/declaration.ipp"

//  PRIVATE METHODS  //

Sabre::Syntax::Node *Sabre::Parser::Dispatch::m_declaration(Stream *parser, Extent extent) {
  switch (parser->current()->kind()) {
  // handle decorators, attributes differently
  case Lexer::Kind::PUNC_DECOR: $_FALLTHROUGH;
  case Lexer::Kind::PUNC_ATTR: return m_terminates(parser, m_preamble(parser, extent), "a declaration");

  // handle baseline modifiers for declarations
  case Lexer::Kind::MOD_PUBLIC: $_FALLTHROUGH;
  case Lexer::Kind::MOD_PRIVATE: $_FALLTHROUGH;
  case Lexer::Kind::MOD_PROTECTED: return m_terminates(parser, m_modifiers(parser, extent), "a declaration");

  // although export/import are statements, we can only parse them at the top - level
  case Lexer::Kind::MOD_IMPORT: return m_terminates(parser, m_import(parser, extent), "an import statement");
  case Lexer::Kind::MOD_EXPORT: return m_terminates(parser, m_export(parser, extent), "an export statement");

  // handle baseline subjects as necessary
  case Lexer::Kind::DECL_ENUM: return m_terminates<Syntax::Enum>(parser, "an enum declaration");
  case Lexer::Kind::DECL_TYPE: return m_terminates<Syntax::Alias>(parser, "a type declaration");
  case Lexer::Kind::DECL_CLASS: return m_terminates<Syntax::Class>(parser, "a class declaration");
  case Lexer::Kind::DECL_SPACE: return m_terminates<Syntax::Namespace>(parser, "a namespace declaration");

  // handle the various top-level declarations
  case Lexer::Kind::DECL_LET: $_FALLTHROUGH;
  case Lexer::Kind::DECL_USE: $_FALLTHROUGH;
  case Lexer::Kind::DECL_MUT: return m_terminates<Syntax::Variable>(parser, "a variable declaration");

  // otherwise attempt matching a suitable statement instead
  default: return m_statement(parser);
  }
}

Sabre::Syntax::Node *
Sabre::Parser::Dispatch::m_terminates(Stream *parser, Syntax::Node *node, const $::String::View &describe) {
  if (node == nullptr || parser->match(Lexer::Kind::PUNC_TERM)) return node;
  return parser->report(node, 2000101, "';'", describe); // invalid terminator
}
