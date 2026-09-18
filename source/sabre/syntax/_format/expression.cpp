/// Format Includes
#include "sabre/format/_inline/macros.ipp"

/// Syntax Includes
#include "sabre/syntax/_inline/generator.ipp"

//  PRIVATE METHODS  //

Sabre::Format::Piece *Sabre::Format::Dispatch::m_expression(Reader *reader, Precedence rank) {
  return Syntax::Generator::expression<Piece, Reader, Dispatch>(reader, rank, [](Reader *reader, auto...) {
    return reader->bail(); // allow bailing out when necessary
  });
}

Sabre::Format::Piece *Sabre::Format::Dispatch::m_assignment(Reader *reader, Piece *target) {
  return m_assignment(reader, target, [](Reader *reader) { return m_expression(reader); });
}

Sabre::Format::Piece *Sabre::Format::Dispatch::m_assignment(Reader *reader, Piece *target, Callback &&callback) {
  // match any incoming assignable token
  if (target == nullptr || !reader->match(Lexer::Flag::ASSIGNS)) return target;

  // prepare the underlying node storage now
  auto *storage = reader->storage();

  // get a unicode copy of the assignment now
  auto token = storage->unicode(reader->previous()->lexeme());

  // attempt parsing the resulting assignment now
  auto *expression = m_leading(reader, std::move(callback));
  if (expression == nullptr) return nullptr; // failed to parse

  // construct the necessary output now to be used
  expression = storage->indent(storage->space().hard, expression);
  return storage->append(target, storage->space().hard, token, expression);
}
