/// Sabre Includes
#include "sabre/parser/delimited.hpp"

/// Syntax Includes
#include "sabre/syntax/_inline/declaration.ipp"
#include "sabre/syntax/_inline/statement.ipp"

//  PRIVATE METHODS  //

Sabre::Syntax::Node *Sabre::Parser::Dispatch::m_import(Stream *parser, Extent extent) {
  auto *result = m_statement<Syntax::Import>(parser);
  if ($_LIKELY(extent == Extent::MODULE)) return result;
  return parser->report(result, 8000202, "Imports");
}

Sabre::Syntax::Node *Sabre::Parser::Dispatch::m_export(Stream *parser, Extent extent) {
  // eat the current token now for use
  auto *token = m_assert(parser->current(), Lexer::Kind::MOD_EXPORT);

  // if we are not within a module context, allow then error-out
  if ($_UNLIKELY(extent != Extent::MODULE)) parser->report(token, 8000202, "Exports");

  // check if we have a leading string for module exports
  switch (parser->peek()->kind()) {
  case Lexer::Kind::LTRL_STR: return m_statement<Syntax::Import>(parser);
  default: parser->advance(); break; // should be suitably breakable here
  }

  // otherwise we attempt reading an item with modifiers now
  auto *declaration = m_modifiers(parser, extent);
  if (declaration == nullptr) return nullptr;

  // prepare the modifiers to be updated
  auto &modifiers = declaration->modifiers();

  // emplace onto the modifiers now
  if (!modifiers.test(Variable::Flag::EXPORT)) modifiers.set(Variable::Flag::EXPORT);
  else parser->report(token, 2000900, token->lexeme()); // declare as a duplicate now

  // and return the resulting declaration details now
  return declaration;
}

SABRE_MM_PARSE_STMT(Import, parser) {
  // get the current tokens to be used
  auto snapshot = parser->snapshot();

  auto exported = parser->match(Lexer::Kind::MOD_EXPORT); // prepare context
  if (!exported && !parser->expect(Lexer::Kind::MOD_IMPORT)) return nullptr;

  // attempt parsing the leading path to be used
  if (!parser->consume(Lexer::Kind::LTRL_STR, 2000100, "an import path")) return nullptr;

  // cache the path to be used
  auto *path = parser->previous();

  // if we do not see an "as" token, then we can stop safely
  if (!parser->match(Lexer::Kind::BINOP_AS)) {
    return parser->allocate<Syntax::Import>(snapshot.enclose(path), path, exported);
  }

  // if we have a suitable identifier, then stop early
  if (parser->check(Lexer::Flag::ANNOTATES)) {
    auto identifier = parser->allocate<Syntax::Identifier>(parser->advance());
    return parser->allocate<Syntax::Import>(snapshot.enclose(path), path, identifier, exported);
  }

  // prepare some details for parsring patterns
  static constexpr auto s_opening = Lexer::Kind::PUNC_LBRACE;
  static constexpr auto s_closing = Lexer::Kind::PUNC_RBRACE;
  static constexpr auto s_callback = [](auto *_) { return m_identifier(_); };
  static auto s_enclosed = Enclosed<Syntax::Identifier, s_opening, s_closing>(s_callback);

  // ensure the instance was happily parsed
  auto patterns = s_enclosed.dispatch(parser);
  if (!patterns.has_value()) return nullptr;

  // ensure that we have some patterns to actually declare
  if (patterns->empty()) return parser->report(path, 8000200);

  // resolve the final import declaration
  return parser->allocate<Syntax::Import>(snapshot.enclose(path), path, *patterns, exported);
}
