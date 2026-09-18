/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_FORMAT_DECL(Node, reader) {
  switch (auto kind = reader->current()->kind()) {
  // handle decorators, attributes differently
  case Lexer::Kind::PUNC_DECOR: $_FALLTHROUGH;
  case Lexer::Kind::PUNC_ATTR: return m_terminates(reader, m_preamble(reader));

  // handle baseline modifiers for declarations
  case Lexer::Kind::MOD_PUBLIC: $_FALLTHROUGH;
  case Lexer::Kind::MOD_PRIVATE: $_FALLTHROUGH;
  case Lexer::Kind::MOD_PROTECTED: return m_terminates(reader, m_modifiers(reader));

  // handle imports/exports as necessary
  case Lexer::Kind::MOD_IMPORT: return m_terminates<Syntax::Import>(reader);
  case Lexer::Kind::MOD_EXPORT: return m_terminates(reader, m_export(reader));

  // handle any potential subjects here
  case Lexer::Kind::DECL_LET: $_FALLTHROUGH;
  case Lexer::Kind::DECL_USE: $_FALLTHROUGH;
  case Lexer::Kind::DECL_MUT: $_FALLTHROUGH;
  case Lexer::Kind::DECL_TYPE: $_FALLTHROUGH;
  case Lexer::Kind::DECL_ENUM: $_FALLTHROUGH;
  case Lexer::Kind::DECL_CLASS: $_FALLTHROUGH;
  case Lexer::Kind::DECL_SPACE: return m_terminates(reader, m_subject(reader, kind));

  // otherwise attempt matching a suitable statement instead
  default: return m_statement(reader);
  }
}

Sabre::Format::Piece *Sabre::Format::Dispatch::m_subject(Reader *reader) {
  return m_subject(reader, reader->peek()->kind());
}

Sabre::Format::Piece *Sabre::Format::Dispatch::m_subject(Reader *reader, Lexer::Kind kind) {
  switch (kind) {
  case Lexer::Kind::DECL_LET: $_FALLTHROUGH;
  case Lexer::Kind::DECL_USE: $_FALLTHROUGH;
  case Lexer::Kind::DECL_MUT: return m_declaration<Syntax::Variable>(reader);

  case Lexer::Kind::DECL_TYPE: return m_declaration<Syntax::Alias>(reader);
  case Lexer::Kind::DECL_ENUM: return m_declaration<Syntax::Enum>(reader);
  case Lexer::Kind::DECL_CLASS: return m_declaration<Syntax::Class>(reader);
  case Lexer::Kind::DECL_SPACE: return m_declaration<Syntax::Namespace>(reader);
  default: return nullptr; // stop if we have an invalid subject here
  }
}

Sabre::Format::Piece *Sabre::Format::Dispatch::m_modifiers(Reader *reader, Variable::Modifiers modifiers) {
  // attempt matching modifiers whilst possible to do so
  while (reader->match(Lexer::Flag::MODIFIER)) {
    switch (reader->previous()->kind()) {
#define SABRE_XX_TOKEN_MODIFIER(K, N, ...)                            \
  case Lexer::Kind::MOD_##K: modifiers.set(Variable::Flag::K); break;
      SABRE_XX_TOKEN_MODIFIER(EXPORT, "export")
#include "sabre/lexer/_defines/tokens.def"

    // ignore all other types of tokens
    default: break;
    }
  }

  // stop if the modifiers are currently empty
  if (modifiers.none()) return m_subject(reader);

  // otherwise we want to build the callback to be used now
  auto callback = [modifiers](Reader *reader) -> Piece * {
    auto *subject = m_subject(reader);
    if (subject == nullptr) return nullptr;

    // prepare the list to be output now
    auto *storage = reader->storage();
    auto *group = storage->group();

    // and construct our desired output as necessary
#define SABRE_XX_TOKEN_MODIFIER(N, T, ...)                                                \
  if (modifiers.test(Variable::Flag::N)) storage->append(group, storage->unicode(T " "));
    SABRE_XX_TOKEN_MODIFIER(EXPORT, "export")
#include "sabre/lexer/_defines/tokens.def"

    // and construct the result now
    return storage->append(group, subject);
  };

  // and attempt flushing leading space now as necessary
  return m_leading(reader, Callback(callback));
}
