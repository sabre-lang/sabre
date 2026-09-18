/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_FORMAT_PREFIX(Tuple, reader) {
  // ensure that the incoming tuple is validated
  if (!reader->match(Lexer::Kind::PUNC_LBRACK)) return nullptr;

  // attempt parsing out the collection required now
  auto callback = [](Reader *reader) -> Piece * { return m_expression(reader); };
  auto *values = m_delimited(reader, {std::move(callback), Lexer::Kind::PUNC_RBRACK});
  if (values == nullptr || !reader->match(Lexer::Kind::PUNC_RBRACK)) return nullptr;

  // prepare the underlying storage now
  auto *storage = reader->storage();

  // prepare the necessary indentation now
  values = storage->indent(storage->line().soft, values);

  // prepare the leading and trailing details to be used
  auto *trailing = storage->list(storage->line().soft, storage->brack().right);

  // and construct the resulting group to be returned now
  return storage->group(storage->brack().left, values, trailing);
}
