/// Syntax Includes
#include "sabre/syntax/_inline/declaration.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_DECL(Internal, parser) {
  // prepare the incoming snapshot
  auto snapshot = parser->snapshot();

  // get the original token location to be used
  auto *token = m_assert(parser->advance(), "Internal");

  // ensure there is now a period available
  if (!parser->expect(Lexer::Kind::PUNC_PERIOD)) return nullptr;

  // attempt pulling out the current feature now
  auto lexeme = parser->current()->lexeme();
  auto feature = Reflect::Inspect::feature(lexeme);

  // check if the found feature is invalid at all before continuing
  if (feature != Reflect::Feature::INVALID) parser->advance();
  else return parser->report(2000802, lexeme); // not valid

  // otherwise can safely construct the necessary node
  return parser->allocate<Syntax::Internal>(snapshot.enclose(token), feature);
}
