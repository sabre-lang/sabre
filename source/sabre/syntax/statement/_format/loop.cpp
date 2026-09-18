/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_FORMAT_STMT(Loop, reader, ) {
  // ensure we have the leading loop value now
  if (!reader->match(Lexer::Kind::LOOP_ANY)) return nullptr;

  // prepare the storage instance
  auto *storage = reader->storage();

  // prepare the header value to be used
  auto *header = storage->group(storage->unicode("loop"));

  // check if we have a leading parenthesis at all
  if (reader->check(Lexer::Kind::PUNC_LPAREN)) {
    auto *condition = m_conditional(reader);
    if (condition == nullptr) return nullptr;
    storage->append(header, storage->space().hard, condition);
  }

  // check if we require an incoming brace at all now
  if (!reader->check(Lexer::Kind::PUNC_LBRACE)) storage->append(header, storage->space().hard);

  // prepare the callback for the statement to be used
  auto callback = [header](Reader *reader) { return m_statement(reader, header); };

  // should be able to consume a statement now for showing
  auto *statement = m_leading(reader, Callback(callback));

  // return the resulting group now
  return statement ? storage->group(header, storage->indent(statement)) : nullptr;
}

SABRE_MM_FORMAT_STMT(For, reader, ) {
  // ensure we remove the leading loop value now
  if (!reader->match(Lexer::Kind::LOOP_FOR)) return nullptr;
  if (!reader->match(Lexer::Kind::PUNC_LPAREN)) return nullptr;

  // prepare the storage instance
  auto *storage = reader->storage();

  // prepare the header value to be used
  auto *prefix = storage->list(storage->unicode("for"), storage->space().hard, storage->paren().left);

  // attempt eating a collection of arguments now
  Callback callback = [](Reader *reader) -> Piece * { return m_identifier(reader); };
  auto *parameters = m_collection(reader, {storage->comma(), std::move(callback), Lexer::Kind::BINOP_IN});
  if (parameters == nullptr || !reader->match(Lexer::Kind::BINOP_IN)) return nullptr; // ensure validated

  // prepare the expression collection to be used now
  auto collection = Collection();

  // append all the leading whitespace
  for (auto *comment : m_leading(reader)) collection.whitespace(comment);

  auto *iterable = m_expression(reader); // parse the incoming iterable
  if (iterable) iterable = storage->append(storage->unicode("in "), iterable);

  // attempt parsing the incoming item now
  if (!collection.append(iterable, m_trailing(reader))) return nullptr;

  // ensure there is a trailing parenthesis now
  if (!reader->match(Lexer::Kind::PUNC_RPAREN)) return nullptr;

  // start re-constructing our header to be used now
  parameters = storage->indent(storage->line().soft, parameters);
  iterable = storage->indent(storage->line().dynamic, collection.merge(storage));

  // join the entire header together again now
  auto *header = storage->group(prefix, parameters, iterable, storage->line().soft, storage->paren().right);

  // check if we are going to have a block-statement
  if (!reader->check(Lexer::Kind::PUNC_LBRACE)) storage->append(header, storage->space().hard);

  // prepare the callback for the statement to be used
  callback = [header](Reader *reader) { return m_statement(reader, header); };

  // should be able to consume a statement now for showing
  auto *statement = m_leading(reader, Callback(callback));

  // return the resulting group now
  return statement ? storage->group(header, storage->indent(statement)) : nullptr;
}
