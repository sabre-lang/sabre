/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PUBLIC METHODS  //

SABRE_MM_FORMAT_INFIX(Ternary, reader, condition) {
  // ensure we parse the incoming ternary token
  if (!reader->match(Lexer::Kind::PUNC_QUERY)) return nullptr;

  // prepare the storage to be used
  auto *storage = reader->storage();

  // prepare a callback factory for our consequence instance
  auto factory = [](Piece *prefix) -> Callback {
    return [prefix](Reader *reader) -> Piece * {
      auto *expression = m_expression(reader);
      if (expression == nullptr) return nullptr;

      auto *storage = reader->storage(); // build the resulting output now
      return storage->append(prefix, storage->space().hard, expression);
    };
  };

  // parse the incoming consequence value
  auto *consequence = m_leading(reader, factory(storage->query()));
  if (consequence == nullptr) return nullptr; // failed to parse

  // ensure we have the separator between our values
  if (!reader->match(Lexer::Kind::PUNC_COLON)) return nullptr;

  // and parse the alternative value now
  auto *alternative = m_leading(reader, factory(storage->colon()));
  if (alternative == nullptr) return nullptr; // failed to parse

  // and attempt building the necessary details now
  auto *separator = storage->line().dynamic;
  Piece::List *ternary = storage->group(condition);

  // bind the consequence and the alternative now
  storage->append(ternary, storage->indent(separator, consequence));
  storage->append(ternary, storage->indent(separator, alternative));

  // return the resulting ternary instance now
  return ternary;
}
