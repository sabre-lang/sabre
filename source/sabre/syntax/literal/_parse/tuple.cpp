/// Sabre Includes
#include "sabre/parser/delimited.hpp"

/// Syntax Includes
#include "sabre/syntax/_inline/expression.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_PREFIX(Tuple, parser, ) {
  // prepare some details for the enclosing parser
  static constexpr auto s_opening = Lexer::Kind::PUNC_LBRACK;
  static constexpr auto s_closing = Lexer::Kind::PUNC_RBRACK;
  static constexpr auto s_callback = [](auto *_) { return m_expression(_); };
  static auto s_enclosed = Enclosed<Syntax::Expression, s_opening, s_closing>(s_callback);

  // get the snapshot to be used now
  auto snapshot = parser->snapshot();

  // attempt parsing the incoming elements now
  auto elements = s_enclosed.dispatch(parser);
  if (!elements.has_value()) return nullptr;

  // construct the resulting list to be used
  return parser->allocate<Syntax::Tuple>(snapshot.bounds(), *elements);
}
