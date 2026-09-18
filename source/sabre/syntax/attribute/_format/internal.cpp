/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_FORMAT_DECL(Internal, reader) {
  // ensure there is now a period available
  if (!reader->match(Lexer::Kind::PUNC_PERIOD)) return nullptr;

  // attempt getting any incoming operator to be used
  auto *identifier = m_identifier(reader);
  if (identifier == nullptr) return nullptr;

  // prepare the storage to be used
  auto *storage = reader->storage();

  // and construct the resulting details
  return storage->append(storage->period(), identifier);
}
