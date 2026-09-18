/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_FORMAT_DECL(Fallback, reader) {
  if (!reader->check(Lexer::Kind::BINOP_MUL)) return m_expression(reader);
  return reader->storage()->unicode(reader->advance()->lexeme()); // found
}

SABRE_MM_FORMAT_STMT(Match, reader, ) {
  // ensure there is a leading conditional token now
  if (!reader->match(Lexer::Kind::FLOW_MATCH)) return nullptr;

  // attempt parsing the value with whitespace now
  auto *value = m_conditional(reader);
  if (value == nullptr) return nullptr;

  // ensure we have a leading brace now
  if (!reader->match(Lexer::Kind::PUNC_LBRACE)) return nullptr;

  // attempt eating the incoming collection now
  auto callback = [](Reader *reader) -> Piece * { return m_statement<Syntax::Query>(reader); };
  auto *queries = m_delimited(reader, {std::move(callback), Lexer::Kind::PUNC_RBRACE});
  if (queries == nullptr || !reader->match(Lexer::Kind::PUNC_RBRACE)) return nullptr;

  // prepare the base storage instance
  auto *storage = reader->storage();

  // prepare the header to be used now with the base details to be shown
  Piece::List *header = storage->group(storage->unicode("match"));
  header = storage->append(header, storage->space().hard, value);
  header = storage->append(header, storage->space().hard, storage->brace().left);

  // stop early if our queries are empty at all
  if (queries->empty()) return storage->append(header, storage->brace().right);

  // otherwise we want to append with an expansion here (always)
  queries = storage->append(queries, storage->expand());
  queries = storage->indent(storage->line().hard, queries);

  // finally merge all our items back together now as needed
  return storage->append(header, queries, storage->line().hard, storage->brace().right);
}

SABRE_MM_FORMAT_STMT(Query, reader, ) {
  // attempt parsing all the incoming arguments now
  auto *guards = m_delimited(reader, {std::move(m_declaration<Syntax::Fallback>), Lexer::Kind::PUNC_COLON});
  if (guards == nullptr || !reader->match(Lexer::Kind::PUNC_COLON)) return nullptr;

  // after the guards, we want to attempt finding a suitable statement to be bound
  auto *statement = m_statement(reader, Syntax::Unterminated());
  if (statement == nullptr) return nullptr;

  // prepare the storage instance to be used
  auto *storage = reader->storage();

  // return the resulting instance to be used now
  statement = storage->indent(storage->space().soft, statement);
  return storage->group(guards, storage->colon(), statement);
}
