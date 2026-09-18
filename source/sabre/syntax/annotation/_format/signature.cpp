/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

Sabre::Format::Piece *Sabre::Format::Dispatch::m_signature(Reader *reader, bool compressed) {
  // ensure we have a leading "fn" keyword
  if (!reader->match(Lexer::Kind::DECL_FUNC)) return nullptr;

  // attempt constructing the base prototype now
  auto *prototype = m_constructor(reader, compressed);
  if (prototype == nullptr) return nullptr;

  // prepare the storage instance now
  auto *storage = reader->storage();
  auto *space = storage->space().hard;

  // ensure the prototype is safely prepended with a space when needed
  if (!prototype->is<Piece::Empty>()) prototype = storage->append(space, prototype);

  // rebuild the prototype to include the "fn" keyword
  prototype = storage->group(storage->unicode("fn"), prototype);

  // prepare the incoming colon-typing now
  Piece *colon = nullptr;

  // if we do not have a colon, then ignore the return value
  if (reader->match(Lexer::Kind::PUNC_COLON)) colon = storage->colon();
  else if (!reader->match(Lexer::Kind::ARROW_THIN)) return prototype;
  else colon = storage->list(space, storage->arrow().thin);

  // otherwise we want to parse the incoming annotation again
  auto callback = [](Reader *reader) { return m_annotation(reader); };
  auto *returns = m_leading(reader, Callback(callback)); // and parse

  // update the returns instance to be it's own grouping
  return returns ? storage->append(prototype, colon, space, returns) : nullptr;
}

SABRE_MM_FORMAT_HINT(Signature, reader) { return m_signature(reader, true); }
SABRE_MM_FORMAT_DECL(Signature, reader) { return m_signature(reader, false); }
