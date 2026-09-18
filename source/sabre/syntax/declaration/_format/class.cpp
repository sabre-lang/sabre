/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

Sabre::Format::Piece *Sabre::Format::Dispatch::m_extends(Reader *reader) {
  // prepare the storage instance
  auto *storage = reader->storage();

  // stop if there is no valid declaration
  if (!reader->match(Lexer::Kind::ARROW_BOLD)) return storage->empty();

  // prepare a callback for incoming super expressions
  static constexpr auto s_callback = [](Reader *reader) { return m_expression(reader); };

  // attempt parsing the incoming expression now
  auto *expression = m_leading(reader, Callback(s_callback));

  // prepare the baseline tokens now
  auto *arrow = storage->arrow().bold;
  auto *space = storage->space().hard;

  // construct the resulting expression details now
  return expression ? storage->append(space, arrow, space, expression) : nullptr;
}

Sabre::Format::Piece *Sabre::Format::Dispatch::m_implements(Reader *reader) {
  // prepare the storage instance
  auto *storage = reader->storage();

  // stop if there is no valid declaration
  if (!reader->match(Lexer::Kind::DECL_IMPL)) return storage->empty();

  // prepare the baseline token now
  auto *space = storage->space().hard;
  auto *prefix = storage->unicode(reader->previous()->lexeme());

  // attempt parsing out the collection required now
  auto *implements = m_delimited(reader, {m_identifier, Lexer::Kind::PUNC_LBRACE});

  // bind the "implements" token as well
  return implements ? storage->group(space, prefix, space, implements) : nullptr;
}

SABRE_MM_FORMAT_DECL(Header, reader) {
  // attempt parsing the incoming constructor
  auto *constructor = m_declaration<Syntax::Constructor>(reader);
  if (constructor == nullptr) return nullptr; // failed to parse

  // get the baseline storage details
  auto *storage = reader->storage();

  // append the header now with the incoming constructor
  auto *header = storage->list(constructor);

  // check for a suitable extension typing now
  auto super = m_extends(reader);
  if (super == nullptr) return nullptr;
  storage->append(header, super);

  // attempt parsing the implementation interfaces
  auto *implements = m_implements(reader);
  if (implements == nullptr) return nullptr;
  else storage->append(header, implements);

  // finally return the underlying header now
  return header;
}

SABRE_MM_FORMAT_DECL(Class, reader) {
  // ensure we eat the leading class details
  if (!reader->match(Lexer::Kind::DECL_CLASS)) return nullptr;

  // get the baseline storage details
  auto *storage = reader->storage();

  // prepare the underlying prefix piece to be used
  Piece *prefix = storage->unicode(reader->previous()->lexeme());

  // attempt getting a suitable identifier
  auto *name = m_identifier(reader);
  if (name == nullptr) return nullptr;

  // attempt parsing the incoming header now
  auto *header = m_declaration<Syntax::Header>(reader);
  if (header == nullptr) return nullptr; // failed

  // prepare the baseline group for the namespace now
  auto *group = storage->group(prefix, storage->space().hard, name, header);

  // attempt parsing the incoming block as well
  auto *block = m_block(reader, group);
  if (block == nullptr) return nullptr;

  // finally cnostruct the end result to be returned
  return storage->append(group, storage->append(block, storage->expand()));
}
