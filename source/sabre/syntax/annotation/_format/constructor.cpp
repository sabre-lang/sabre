/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

Sabre::Format::Piece *Sabre::Format::Dispatch::m_constructor(Reader *reader, bool compressed) {
  // attempt parsing the generics available
  auto *generics = m_template(reader);
  if (generics == nullptr) return nullptr;

  // attempt parsing the incoming parameters now
  auto *parameters = m_parameters(reader, compressed);
  if (parameters == nullptr) return nullptr;

  // prepare the storage instance to be used
  auto *storage = reader->storage();

  // stop early if either are empty now
  if (generics->is<Piece::Empty>() && parameters->is<Piece::Empty>()) return storage->empty();

  // and construct the resulting constructor to be used now
  return storage->group(generics, parameters);
}

SABRE_MM_FORMAT_HINT(Constructor, reader) { return m_constructor(reader, true); }
SABRE_MM_FORMAT_DECL(Constructor, reader) { return m_constructor(reader, false); }
