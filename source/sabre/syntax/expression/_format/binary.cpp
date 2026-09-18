/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PUBLIC METHODS  //

Sabre::Format::Piece *Sabre::Format::Dispatch::m_binary(Reader *reader, Piece *left, Callback &&callback) {
  // get the underlying storage instance
  auto *storage = reader->storage();

  // prepare the initial token to be used
  auto token = (reader->skip(), storage->unicode(reader->advance()->lexeme()));

  // attempt eating the right-hand side argument now
  auto *right = m_leading(reader, std::move(callback));
  if (right == nullptr) return nullptr; // failed reading

  // and construct the resulting output now
  right = storage->group(storage->indent(storage->space().hard, right));
  return storage->append(left, storage->space().hard, token, right);
}

SABRE_MM_FORMAT_INFIX(Binary, reader, left) {
  return m_binary(reader, left, [](Reader *reader) { return m_expression(reader); });
}
