/// Syntax Includes
#include "sabre/syntax/_inline/declaration.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_DECL(Alias, parser) {
  // prepare the snapshot to be used
  auto snapshot = parser->snapshot();

  // ensure there is an underlying type declaration
  m_assert(parser->advance(), Lexer::Kind::DECL_TYPE);

  // ensure we have a valid alias identifier
  auto *name = m_label(parser);
  if (name == nullptr) return nullptr;

  // attempt parsing optional generics here
  auto generics = m_template(parser);
  if (!generics.has_value()) return nullptr;

  // ensure there is an assignment token now
  if (!parser->expect(Lexer::Kind::ASOP_DEF)) return nullptr;

  // parse the incoming type-alias now
  auto *hint = m_annotation(parser);
  if (hint == nullptr) return nullptr;

  // construct the resulting alias (do not declare since type only)
  return parser->allocate<Syntax::Alias>(snapshot.enclose(name), name->lexeme(), hint, *generics);
}
