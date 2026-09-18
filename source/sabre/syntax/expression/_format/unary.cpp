/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PUBLIC METHODS  //

SABRE_MM_FORMAT_PREFIX(Unary, reader) {
  // get the underlying storage instance
  auto *storage = reader->storage();

  // pre-parse the unary expression to be used now
  auto token = storage->unicode(reader->advance()->lexeme());

  // attempt parsing the incoming expression now
  auto callback = [](Reader *reader) { return m_expression(reader); };
  auto *expression = m_leading(reader, Callback(callback));
  if (expression == nullptr) return nullptr; // failed reading

  // should be able to construct a suitable output now
  return storage->group(token, storage->indent(storage->line().soft, expression));
}
