/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_FORMAT_STMT(Return, reader, ) {
  // prepare the storage instance
  auto *storage = reader->storage();

  // prepare the leading return token now
  auto keyword = storage->unicode(reader->advance()->lexeme());

  // check for immediately stoppable values
  if (reader->check(Lexer::Kind::MISC_MAX)) return keyword;
  else if (reader->check(Lexer::Flag::TRAILING)) return keyword;

  // prepare a suitable callback for the panic-handler
  auto callback = [](Reader *reader) { return m_expression(reader); };
  auto *expression = m_leading(reader, Callback(callback));
  if (expression == nullptr) return expression; // invalid parse

  // construct the resulting panic output now
  return storage->group(keyword, storage->space().hard, expression);
}
