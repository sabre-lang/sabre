/// Syntax Includes
#include "sabre/syntax/_inline/annotation.ipp"

//  PRIVATE METHODS  //

Sabre::Syntax::Signature *Sabre::Parser::Dispatch::m_signature(Stream *parser, bool annotated) {
  // prepare the snapshot to be used
  auto snapshot = parser->snapshot();

  // ensure we have an opening funtion to be used now
  if (!parser->consume(Lexer::Kind::DECL_FUNC, 2000100, "a function signature")) return nullptr;

  // prepare the location to be inherited
  auto *token = parser->previous();

  // attempt parsing the base prototype to be used
  auto *base = m_constructor(parser, annotated);
  if (base == nullptr) return nullptr; // failed

  auto kind = annotated ? Lexer::Kind::ARROW_THIN : Lexer::Kind::PUNC_COLON;
  auto colon = parser->match(kind); // check for incoming return-typing now

  auto *returns = colon ? m_annotation(parser) : nullptr;
  if (returns == nullptr && colon) return nullptr;

  // update the returns if we have an annotation here
  if (returns == nullptr && annotated) returns = m_qualifier(parser, "Any");

  // construct the resulting signature now
  return parser->allocate<Syntax::Signature>(snapshot.enclose(token), base, returns);
}

SABRE_MM_PARSE_HINT(Signature, parser) { return m_signature(parser, true); }
SABRE_MM_PARSE_DECL(Signature, parser) { return m_signature(parser, false); }
