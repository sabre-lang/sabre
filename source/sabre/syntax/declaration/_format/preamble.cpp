/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

Sabre::Format::Piece *Sabre::Format::Dispatch::m_category(Reader *reader) {
  // attempt parsing based on the incoming category now
  if (!reader->match(Lexer::Flag::ANNOTATES)) return nullptr;

  // get the current category token now
  auto *storage = reader->storage();
  auto *category = reader->previous();

#define X(_, N, L, ...)                                                                               \
  case XH::FNV::U32(L): {                                                                             \
    auto *attribute = m_declaration<Syntax::N>(reader); /** attempt parsing the incoming attribute */ \
    if (attribute != nullptr) return storage->append(storage->unicode(L), attribute);                 \
  } break;

  // attempt handling the incoming categories now
  switch (XH::FNV::U32(category->lexeme())) { SABRE_XX_REFLECTION_CATEGORIES(X) default : break; }
#undef X

  // failures should always exit here
  return nullptr;
}

Sabre::Format::Piece *Sabre::Format::Dispatch::m_attribute(Reader *reader) {
  // ensure we have a valid starting token now
  if (!reader->match(Lexer::Kind::PUNC_ATTR)) return nullptr;
  if (!reader->match(Lexer::Kind::PUNC_LBRACK)) return nullptr;

  // attempt parsing based on the incoming category now
  auto *category = m_category(reader);
  if (category == nullptr) return nullptr;

  // ensure there is a closing bracket as well
  if (!reader->match(Lexer::Kind::PUNC_RBRACK)) return nullptr;

  // get the storage instance now
  auto *storage = reader->storage();

  // can validly construct the result now
  return storage->group(storage->attr(), category, storage->brack().right);
}

Sabre::Format::Piece *Sabre::Format::Dispatch::m_decorator(Reader *reader) {
  // ensure we have a valid starting token now
  if (!reader->match(Lexer::Kind::PUNC_DECOR)) return nullptr;

  // prepare the storage instance
  auto *storage = reader->storage();

  // parse the incoming expression to be used now
  auto callback = [](Reader *reader) { return m_expression(reader); };
  auto *expression = m_leading(reader, Callback(callback));

  // handle the outgoing result to be emitted
  return expression ? storage->group(storage->decor(), expression) : nullptr;
}

Sabre::Format::Piece *Sabre::Format::Dispatch::m_preamble(Reader *reader) {
  // prepare the attribute and decorator pieces
  auto attributes = Buffer(), decorators = Buffer();

  // whilst we can match attributes, we parse them as necessary
  while (reader->check(Lexer::Flag::DECORATES)) {
    if (reader->check(Lexer::Kind::PUNC_ATTR)) attributes.emplace_back(m_attribute(reader));
    else if (reader->check(Lexer::Kind::PUNC_DECOR)) decorators.emplace_back(m_decorator(reader));
  }

  // check if either are invalid at all (this means we should stop formatting)
  if (std::ranges::contains(attributes, nullptr) || std::ranges::contains(decorators, nullptr)) return nullptr;

  // we now want to attempt parsing our incoming modifiers with leading whitespace
  auto *declaration = m_modifiers(reader);
  if (declaration == nullptr) return nullptr;

  // and finally we merge everything together
  auto *storage = reader->storage();
  auto *preamble = storage->list();

  // prepend the attributes, then the decorators
  for (const auto &attribute : attributes) storage->append(preamble, attribute, storage->line().hard);
  for (const auto &decorator : decorators) storage->append(preamble, decorator, storage->line().hard);

  // finally resolve the declaration to be appended
  return storage->append(preamble, declaration);
}
