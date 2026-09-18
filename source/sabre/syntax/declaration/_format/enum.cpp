/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_FORMAT_DECL(Enum, reader) {
  // ensure we eat the leading "enum" keyword
  if (!reader->match(Lexer::Kind::DECL_ENUM)) return nullptr;

  auto *storage = reader->storage(); // prepare the storage
  Piece *prefix = storage->unicode(reader->previous()->lexeme());

  // attempt getting a suitable identifier
  auto *name = m_identifier(reader);
  if (name == nullptr) return nullptr;

  // ensure there is a leading brace now
  if (!reader->match(Lexer::Kind::PUNC_LBRACE)) return nullptr;

  auto *header = storage->group(prefix, storage->space().hard, name); // prepare header and stype
  auto *style = reader->options()->braces_preserve ? storage->space().hard : storage->line().hard;

  // attempt eating the incoming collection now
  auto callback = [](Reader *reader) -> Piece * { return m_declaration<Syntax::Variant>(reader); };
  auto *variants = m_delimited(reader, {std::move(callback), Lexer::Kind::PUNC_RBRACE});
  if (variants == nullptr || !reader->match(Lexer::Kind::PUNC_RBRACE)) return nullptr;

  // prepare the grouping to be used now
  auto *enumeration = storage->group(header, style, storage->brace().left);

  // if the variants are empty, then stop early
  if (variants->empty()) return storage->append(enumeration, storage->brace().right);

  // otherwise we want to append with an expansion here (always)
  variants = storage->append(variants, storage->expand());
  variants = storage->indent(storage->line().hard, variants);

  // and construct the resulting enumeration instance now
  return storage->append(enumeration, variants, storage->line().hard, storage->brace().right);
}

SABRE_MM_FORMAT_DECL(Variant, reader) {
  // attempt reading a suitable name here
  auto *name = m_identifier(reader);
  if (name == nullptr) return nullptr;

  // start preparing the variant output now
  auto *storage = reader->storage();
  Piece::List *variant = storage->group(name);

  // check for any leading parenthesis
  if (reader->match(Lexer::Kind::PUNC_LPAREN)) {
    // attempt reading the incoming label
    auto *label = m_expression(reader);
    if (label == nullptr) return nullptr;

    // ensure the label is actually closed as well
    if (!reader->match(Lexer::Kind::PUNC_RPAREN)) return nullptr;

    // append the incoming label now as necessary
    storage->append(variant, storage->paren().left, label, storage->paren().right);
  }

  // handle any potential assignment that could occur
  return m_assignment(reader, variant);
}
