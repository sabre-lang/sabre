/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_FORMAT_HINT(Qualifier, reader) {
  // attempt parsing an immediate identifier to be used
  auto *initial = m_identifier(reader);
  if (initial == nullptr) return nullptr;

  // attempt parsing the incoming segments now
  auto *segments = m_chain(reader);
  if (segments == nullptr) return nullptr;

  // and finally handle any template specialization
  return m_specialize(reader, reader->storage()->append(initial, segments));
}
