/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_FORMAT_DECL(Deprecated, reader) {
  // prepare the storage instance now
  auto *storage = reader->storage();

  // attempt returning a valid token when necessary
  if (!reader->match(Lexer::Kind::LTRL_STR)) return storage->empty();

  // can safely return the deprecation string now
  auto *message = storage->unicode(reader->previous()->lexeme(), true);
  return storage->append(storage->space().hard, message);
}
