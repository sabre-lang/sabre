/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PUBLIC METHODS  //

SABRE_MM_FORMAT_INFIX(Execute, reader, callee) {
  // ignore if there is no valid policy available
  if (!reader->match(Lexer::Kind::PUNC_POLICY)) return nullptr;

  // prepare the storage instance
  auto *storage = reader->storage();

  // attempt capturing a suitable policy value
  auto *policy = m_identifier(reader);

  // update the policy to asynchronous if missing at all
  if (policy == nullptr) policy = storage->unicode("async");

  // append the ":" token now
  callee = storage->append(callee, storage->policy(), policy);

  // attempt resolving the resulting infix handler now
  return m_infix<Syntax::Call>(reader, callee);
}
