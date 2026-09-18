/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PUBLIC METHODS  //

SABRE_MM_FORMAT_INFIX(Call, reader, callee) {
  // stop if there is no suitable parenthesis for the value
  if (!reader->match(Lexer::Kind::PUNC_LPAREN)) return nullptr;

  // check for final "function" arguments now
  Piece *minimize = nullptr;

  // prepare the callback with minimization handler
  auto callback = [&](Reader *reader) -> Piece * {
    auto cache = reader->check(Lexer::Kind::DECL_FUNC);
    auto *argument = m_expression(reader); // read next
    return minimize = cache ? argument : nullptr, argument;
  };

  // attempt parsing all the incoming arguments now
  auto *arguments = m_delimited(reader, {std::move(callback), Lexer::Kind::PUNC_RPAREN});
  if (arguments == nullptr || !reader->match(Lexer::Kind::PUNC_RPAREN)) return nullptr;

  // prepare the underlying storage instance
  auto *storage = reader->storage();

  // if minimizing, then we zero the final pieces width now (but need to find it first)
  for (auto iter = arguments->pieces().rbegin(); minimize && iter != arguments->pieces().rend(); ++iter) {
    // attempt finding the required item
    if (*iter != minimize) continue;

    *iter = storage->minimize(minimize);
    break; // stop handling since found
  }

  // only allow indentation of arguments if given then
  if (!arguments->empty()) {
    arguments = storage->indent(storage->line().soft, arguments);
    arguments = storage->append(arguments, storage->line().soft);
  }

  // construct the resulting parameters group
  return storage->group(callee, storage->paren().left, arguments, storage->paren().right);
}
