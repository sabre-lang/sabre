/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PUBLIC METHODS  //

SABRE_MM_FORMAT_PREFIX(Group, reader) {
  // ensure we match the leading parenthesis
  if (!reader->match(Lexer::Kind::PUNC_LPAREN)) return nullptr;

  // attempt parsing the incoming grouped expression now
  auto callback = [](Reader *reader) { return m_expression(reader); };
  auto *expression = m_leading(reader, Callback(callback));
  if (expression == nullptr) return nullptr; // failed here

  // and match the trailing parenthesis
  if (!reader->match(Lexer::Kind::PUNC_RPAREN)) return nullptr;

  // prepare the storage container to be used
  auto *storage = reader->storage();

  /// TODO: work out how to implement smart indentation for groups
  // expression = storage->indent(storage->line().soft, expression, storage->line().soft);

  // and join our details together now as necessary
  return storage->group(storage->paren().left, expression, storage->paren().right);
}
