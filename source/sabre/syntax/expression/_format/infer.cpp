/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PUBLIC METHODS  //

SABRE_MM_FORMAT_INFIX(Infer, reader, value) {
  // get the underlying storage instance
  auto *storage = reader->storage();

  // prepare the initial token to be used
  auto token = (reader->skip(), storage->unicode(reader->advance()->lexeme()));

  // check for any potential negation now
  auto *negation = reader->match(Lexer::Kind::UNOP_NOT) ? storage->unicode("!") : nullptr;

  // prepare the callback to be used now
  static auto callback = [](Reader *reader) { return m_annotation(reader); };

  // attempt eating the right-hand side argument now
  auto *right = m_leading(reader, Callback(callback));
  if (right == nullptr) return nullptr; // failed to read

  // and construct the resulting output now
  right = storage->group(storage->indent(storage->space().hard, right));
  return storage->append(value, storage->space().hard, token, negation, right);
}
