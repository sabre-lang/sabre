/// Sabre Includes
#include "sabre/parser/delimited.hpp"

/// Syntax Includes
#include "sabre/syntax/_inline/annotation.ipp"

//  PRIVATE METHODS  //

Sabre::Syntax::Annotation *Sabre::Parser::Dispatch::m_qualifier(Stream *parser, const $::String::View &name) {
  auto *identifier = parser->allocate<Syntax::Identifier>(name);
  return parser->allocate<Syntax::Qualifier>(identifier);
}

SABRE_MM_PARSE_HINT(Qualifier, parser) {
  // prepare the underlying delimited/enclosed handlers
  static constexpr auto s_opening = Lexer::Kind::PUNC_LBRACK;
  static constexpr auto s_closing = Lexer::Kind::PUNC_RBRACK;
  static constexpr auto s_separator = Lexer::Kind::PUNC_PERIOD;
  static constexpr auto s_annotation = [](auto *_) { return m_annotation(_); };
  static constexpr auto s_identifier = [](auto *_) { return m_identifier(_); };
  static auto s_delimited = Delimited<Syntax::Identifier, s_separator>(s_identifier);
  static auto s_enclosed = Enclosed<Syntax::Annotation, s_opening, s_closing>(s_annotation);

  // cache the current range value to be used
  auto snapshot = parser->snapshot();

  // attempt parsing all the available segments now
  auto segments = s_delimited.dispatch(parser);
  if (!segments.has_value()) return nullptr;

  // stop early if there are no types expected
  if (!parser->check(s_opening)) return parser->allocate<Syntax::Qualifier>(snapshot.bounds(), *segments);

  // attempt getting a generic list now as necessary
  auto types = s_enclosed.dispatch(parser);
  if (!types.has_value()) return nullptr;

  // report a failure if the types are empty at all
  if (types->empty()) parser->report(snapshot.bounds(), 2000301);

  // construct the resulting end type now
  return parser->allocate<Syntax::Qualifier>(snapshot.bounds(), *segments, *types);
}
