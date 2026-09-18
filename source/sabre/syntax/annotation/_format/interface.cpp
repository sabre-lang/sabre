/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_FORMAT_HINT(Interface, reader) {
  // ensure we have a leading brace now
  if (!reader->match(Lexer::Kind::PUNC_LBRACE)) return nullptr;

  // attempt eating the incoming collection now
  auto callback = [](Reader *reader) { return m_terminates(reader, m_variable(reader, true)); };
  auto *properties = m_collection(reader, {std::move(callback), Lexer::Kind::PUNC_RBRACE});
  if (properties == nullptr || !reader->match(Lexer::Kind::PUNC_RBRACE)) return nullptr;

  // prepare the storage to be used
  auto *storage = reader->storage();

  // prepare the initial output to be used
  auto *interface = storage->group(storage->brace().left);

  // stop early when possible to do so
  if (properties->empty()) return storage->append(interface, storage->brace().right);

  // ensure we have the necessary indentation as well
  properties = storage->indent(storage->line().dynamic, properties);
  return storage->append(interface, properties, storage->line().dynamic, storage->brace().right);
}
