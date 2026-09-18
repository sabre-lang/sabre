/// Syntax Includes
#include "sabre/syntax/_inline/annotation.ipp"
#include "sabre/syntax/_inline/expression.ipp"
#include "sabre/syntax/_inline/statement.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_PREFIX(Lambda, parser, ) {
  // prepare the entire snapshot to be used now
  auto snapshot = parser->snapshot();

  // attempt parsing the incoming signature value
  auto *signature = m_declaration<Syntax::Signature>(parser);
  if (signature == nullptr) return nullptr; // failed

  // attempt parsing the incoming body as well
  auto *body = [&] -> Syntax::Node * {
    if (parser->check(Lexer::Kind::PUNC_LBRACE)) return m_block(parser, Extent::SCOPING);
    if (!parser->expect(Lexer::Kind::ARROW_BOLD, "a function body")) return nullptr;
    return m_expression(parser); // get the arrow expression to be used instead
  }();

  // ensure the incoming body is actually valid now
  return body ? parser->allocate<Syntax::Lambda>(snapshot.bounds(), signature, body) : nullptr;
}
