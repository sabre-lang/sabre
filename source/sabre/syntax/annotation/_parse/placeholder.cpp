/// Sabre Includes
#include "sabre/parser/delimited.hpp"

/// Syntax Includes
#include "sabre/syntax/_inline/annotation.ipp"

//  PRIVATE METHODS  //

std::optional<Sabre::Syntax::Template> Sabre::Parser::Dispatch::m_template(Stream *parser) {
  // prepare the details to be used to parse a typed expression
  static constexpr auto s_opening = Lexer::Kind::PUNC_LBRACK;
  static constexpr auto s_closing = Lexer::Kind::PUNC_RBRACK;
  static constexpr auto s_callback = m_annotation<Syntax::Placeholder>;
  static auto s_enclosed = Enclosed<Syntax::Placeholder, s_opening, s_closing>(s_callback);

  // return an empty template if no opening given
  if (!parser->check(s_opening)) return Syntax::Template();

  // prepare a snapshot for failures
  auto snapshot = parser->snapshot();

  // attempt parsing our template now
  auto result = s_enclosed.dispatch(parser);
  if (!result.has_value()) return std::nullopt;

  // ensure we have at least one value
  if (result->size()) return result;

  // otherwise declare a failure since we expected at least one type parameter
  return parser->report(snapshot.bounds(), 2000302), result;
}

SABRE_MM_PARSE_HINT(Placeholder, parser) {
  // prepare the snapshot to be used
  auto snapshot = parser->snapshot();

  // get the underlying name of the placeholder
  auto *name = m_label(parser);
  if (name == nullptr) return nullptr;

  auto colon = parser->match(Lexer::Kind::PUNC_COLON); // check for the hint token
  Syntax::Annotation *extends = colon ? m_annotation(parser) : m_qualifier(parser, "Any");
  if (extends == nullptr) return nullptr; // failed to read extends hint so we ignore

  // attempt checking for a valid fallback typing
  auto declare = parser->match(Lexer::Kind::ASOP_DEF); // check for the decl token
  Syntax::Annotation *fallback = declare ? m_annotation(parser) : nullptr;
  if (declare && fallback == nullptr) return nullptr; // failed to read a fallback

  // construct the resulting placeholder now
  return parser->allocate<Syntax::Placeholder>(snapshot.enclose(name), name->lexeme(), extends, fallback);
}
