/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

Sabre::Format::Piece *Sabre::Format::Dispatch::m_conditional(Reader *reader) {
  // stop if there is no suitable parenthesis for the value
  if (!reader->match(Lexer::Kind::PUNC_LPAREN)) return nullptr;

  // prepare a baseline details now
  auto collection = Collection();
  auto *storage = reader->storage();

  // append all the leading whitespace now
  for (auto *comment : m_leading(reader)) collection.whitespace(comment);

  // attempt parsing the incoming item now
  if (!collection.append(m_expression(reader), m_trailing(reader))) return nullptr;

  // ensure there is a trailing parenthesis now
  if (!reader->match(Lexer::Kind::PUNC_RPAREN)) return nullptr;

  // rebuild the collection to include an indentation
  auto *condition = storage->indent(storage->line().soft, collection.merge(storage));

  // join all our details together now
  return storage->group(storage->paren().left, condition, storage->line().soft, storage->paren().right);
}

SABRE_MM_FORMAT_STMT(Conditional, reader, ) {
  // ensure there is a leading conditional token now
  if (!reader->match(Lexer::Kind::FLOW_IF)) return nullptr;

  // attempt parsing the condition with whitespace now
  auto *condition = m_conditional(reader);
  if (condition == nullptr) return nullptr;

  // prepare the base storage instance
  auto *storage = reader->storage();

  // prepare the header to be used now with the "if" details to be shown
  auto *header = storage->group(storage->unicode("if"), storage->space().hard, condition);

  // check if we are going to have a block-statement
  auto same_line = reader->check(Lexer::Kind::PUNC_LBRACE);
  if (!same_line) storage->append(header, storage->space().hard);

  // prepare a callback to be used for statements
  auto callback = [header](Reader *reader) { return m_statement(reader, header); };

  // and parse the incoming block as necessary now
  auto *consequence = m_leading(reader, Callback(callback));
  if (consequence == nullptr) return nullptr;

  // prepare the details for the "if" grouping now to be handled
  header = storage->group(header, storage->indent(consequence));

  // stop if there is no else instance here at all
  if (!reader->match(Lexer::Kind::FLOW_ELSE)) return header;

  // ensure we append the necessary "else" details as well
  storage->append(header, same_line ? storage->space().hard : storage->line().hard, storage->unicode("else"));

  // check for incoming blocks after the else instance
  if (!reader->check(Lexer::Kind::PUNC_LBRACE)) storage->append(header, storage->space().hard);

  // prepare the final statement to be appended now
  auto *alternative = m_leading(reader, Callback(callback));

  // and construct the resulting details now
  return alternative ? storage->group(header, storage->indent(alternative)) : nullptr;
}
