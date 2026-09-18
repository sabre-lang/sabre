/// Syntax Includes
#include "sabre/syntax/_inline/declaration.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_DECL(Overload, parser) {
  // prepare the snapshot to be used
  auto snapshot = parser->snapshot();

  // get the original token location to be used
  auto *token = m_assert(parser->advance(), "Overload");

  // attempt parsing a signature now
  auto *signature = m_signature(parser, true);

  // and resolve as necessary now as necessary
  return signature ? parser->allocate<Syntax::Overload>(snapshot.enclose(token), signature) : nullptr;
}
