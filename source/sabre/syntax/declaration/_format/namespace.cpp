/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_FORMAT_DECL(Namespace, reader) {
  // ensure we eat the leading namespace details
  if (!reader->match(Lexer::Kind::DECL_SPACE)) return nullptr;

  auto *storage = reader->storage(); // prepare the base details
  Piece *prefix = storage->unicode(reader->previous()->lexeme());

  // attempt getting a suitable identifier
  auto *name = m_identifier(reader);
  if (name == nullptr) return nullptr;

  // prepare the baseline group for the namespace now
  auto *header = storage->group(prefix, storage->space().hard, name);

  // attempt parsing the incoming block as well
  auto *block = m_block(reader, header);
  if (block == nullptr) return nullptr;

  // finally construct the end result to be returned (with forced expansion)
  return storage->group(header, storage->append(block, storage->expand()));
}
