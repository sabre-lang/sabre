/// Syntax Includes
#include "sabre/syntax/_inline/declaration.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_DECL(Namespace, parser) {
  // prepare the snapshot
  auto snapshot = parser->snapshot();

  // ensure there is an underlying type declaration
  m_assert(parser->advance(), Lexer::Kind::DECL_SPACE);

  // ensure we have a valid name to be used
  auto *name = m_label(parser);
  if (name == nullptr) return nullptr;

  // parse an expected block for the namespace
  auto *block = m_block(parser, Extent::MODULE);
  if (block == nullptr) return nullptr; // bail

  // and return the resulting namespace now
  return parser->allocate<Syntax::Namespace>(snapshot.enclose(name), name->lexeme(), block);
}
