/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_FORMAT_HINT(Node, reader) {
  if (reader->check(Lexer::Kind::DECL_FUNC)) return m_annotation<Syntax::Signature>(reader);
  else if (reader->check(Lexer::Kind::PUNC_LBRACE)) return m_annotation<Syntax::Interface>(reader);
  else if (reader->check(Lexer::Flag::ANNOTATES)) return m_annotation<Syntax::Qualifier>(reader);

  // failed to produce a valid annotation here so return now
  return nullptr;
}

Sabre::Format::Piece *Sabre::Format::Dispatch::m_specialize(Reader *reader, Piece *qualifier) {
  // ensure we fail immediately if there is no value
  if (qualifier == nullptr) return nullptr;

  // stop early when possible to do so
  if (!reader->match(Lexer::Kind::PUNC_LBRACK)) return qualifier;

  // prepare the underlying node storage
  auto *storage = reader->storage();

  // attempt parsing out the collection required now
  auto callback = [](Reader *reader) -> Piece * { return m_annotation(reader); };
  auto *parameters = m_delimited(reader, {std::move(callback), Lexer::Kind::PUNC_RBRACK});
  if (parameters == nullptr || !reader->match(Lexer::Kind::PUNC_RBRACK)) return nullptr;

  // prepare the necessary indentation now
  parameters = storage->indent(storage->line().soft, parameters);

  // prepare the leading and trailing details to be used
  auto *leading = storage->list(qualifier, storage->brack().left);
  auto *trailing = storage->list(storage->line().soft, storage->brack().right);

  // and construct the resulting group to be returned now
  return storage->group(leading, parameters, trailing);
}