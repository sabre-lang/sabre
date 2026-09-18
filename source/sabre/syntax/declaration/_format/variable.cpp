/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

Sabre::Format::Piece *Sabre::Format::Dispatch::m_variable(Reader *reader, bool compressed) {
  // prepare the storage instance to be used now
  auto *storage = reader->storage();
  Piece::List *variable = storage->group();

  // check for an incoming variable modifier
  auto modifier = reader->match(Lexer::Flag::DEFINES);

  // check that the incoming modifier token was valid
  if (!(modifier || compressed)) return nullptr;

  // check the incoming mutability being used now
  auto *declaration = modifier ? reader->previous() : nullptr;

  // update the variables immutability
  if (!compressed || (declaration && declaration->kind() != Lexer::Kind::DECL_LET)) {
    auto *mutability = storage->unicode(declaration->lexeme());
    storage->append(variable, mutability, storage->space().hard);
  }

  if (auto *name = m_identifier(reader)) storage->append(variable, name);
  else return nullptr; // failed to parse the incoming variable name

  // determine whether or not the item is optional
  if (reader->match(Lexer::Kind::PUNC_QUERY)) storage->append(variable, storage->query());

  // attempt testing for an incoming typing to be appended if found
  if (reader->match(Lexer::Kind::PUNC_COLON)) {
    // prepare the callback to be used for the annotation
    auto callback = [](Reader *reader) { return m_annotation(reader); };

    // and attempt running the leading whitespace handler
    auto *hint = m_leading(reader, Callback(callback));
    if (hint == nullptr) return nullptr; // failed

    // prepare the comments to be associated with the incoming hint
    hint = storage->indent(storage->line().dynamic, hint);
    storage->append(variable, storage->colon(), storage->group(hint));
  }

  // allow assigning the variable regardless of annotation status
  return m_assignment(reader, variable);
}

SABRE_MM_FORMAT_HINT(Variable, reader) { return m_variable(reader); }
SABRE_MM_FORMAT_DECL(Variable, reader) { return m_variable(reader); }
