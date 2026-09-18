/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

Sabre::Format::Piece *Sabre::Format::Dispatch::m_chain(Reader *reader) {
  // prepare the underlying piece storage
  auto *storage = reader->storage();
  auto *segments = storage->indent();

  // prepare the callback to be used for the accessor now
  auto callback = [storage](Reader *reader) -> Piece * {
    // attempt parsing the incoming identifier
    auto *field = m_identifier(reader);
    if (field == nullptr) return nullptr;

    // bind the incoming period with the field now
    return storage->list(storage->period(), field);
  };

  // attempt matching incoming identifiers whilst possible
  while (reader->match(Lexer::Kind::PUNC_PERIOD)) {
    auto *field = m_leading(reader, std::move(callback));
    if (field == nullptr) return nullptr; // failed here
    storage->append(segments, storage->line().soft, field);
  }

  // return the resulting segments as a group now
  return segments->empty() ? storage->empty() : storage->group(segments);
}

SABRE_MM_FORMAT_INFIX(Accessor, reader, parent) {
  // attempt parsing the incoming segments
  auto *segments = m_chain(reader);
  if (segments == nullptr) return nullptr;

  // prepare the underlying piece storage
  auto *storage = reader->storage();

  // merge the leading item and the field to construct the accessor
  auto *accessor = storage->append(parent, segments);

  // handle based on the resulting assignment
  return m_assignment(reader, accessor);
}
