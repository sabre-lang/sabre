/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

Sabre::Format::Piece *Sabre::Format::Dispatch::m_import(Reader *reader, bool exported) {
  // ensure valid incoming details before continuing
  if (!exported && !reader->match(Lexer::Kind::MOD_IMPORT)) return nullptr;

  // cache the path value now
  auto *storage = reader->storage();
  auto *whitespace = storage->list();

  // prepend all the leading whitespace now
  for (const auto &comment : m_leading(reader)) storage->append(whitespace, comment, storage->line().hard);

  // ensure we have an incoming string now
  if (!reader->match(Lexer::Kind::LTRL_STR)) return nullptr;

  auto *path = storage->unicode(reader->previous()->lexeme(), true);
  auto *prefix = storage->unicode(exported ? "export" : "import");
  Piece *header = storage->group(prefix, storage->space().hard, path);

  if (!reader->match(Lexer::Kind::BINOP_AS)) return storage->append(whitespace, header);
  header = storage->append(header, storage->unicode(" as ")); // emplace the "as" token

  // prepend more leading whitespace now
  for (const auto &comment : m_leading(reader)) storage->append(whitespace, comment, storage->line().hard);

  // if we have an incoming identifier
  if (reader->match(Lexer::Flag::ANNOTATES)) {
    auto *pattern = storage->unicode(reader->previous()->lexeme());
    return storage->append(whitespace, header, pattern); // join
  }

  // stop if there is no suitable open brace now
  if (!reader->match(Lexer::Kind::PUNC_LBRACE)) return nullptr;

  // attempt matching incoming patterns now
  auto callback = [](Reader *reader) { return m_identifier(reader); };
  auto *patterns = m_delimited(reader, {std::move(callback), Lexer::Kind::PUNC_RBRACE});
  if (patterns == nullptr || !reader->match(Lexer::Kind::PUNC_RBRACE)) return nullptr;

  // bind the opening brace as necessary
  storage->append(header, storage->brace().left);

  // append the patterns as necessary
  if (!patterns->empty()) {
    patterns = storage->indent(storage->line().dynamic, patterns);
    storage->append(header, patterns, storage->line().dynamic);
  }

  // and construct the resulting import now
  return storage->append(whitespace, storage->append(header, storage->brace().right));
}

Sabre::Format::Piece *Sabre::Format::Dispatch::m_export(Reader *reader) {
  // validate the incoming token to begin with before continuing
  if (!reader->match(Lexer::Kind::MOD_EXPORT)) return nullptr;

  // if we have an incoming string, then drop into an import
  if (reader->check(Lexer::Kind::LTRL_STR)) return m_import(reader, true);

  // build the instance with modifiers now
  return m_modifiers(reader, {Variable::Flag::EXPORT});
}

SABRE_MM_FORMAT_STMT(Import, reader, ) { return m_import(reader, false); }
