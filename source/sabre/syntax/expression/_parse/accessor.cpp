/// Syntax Includes
#include "sabre/syntax/_inline/expression.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_INFIX(Accessor, parser, parent, assignable) {
  // prepare the snapshot to be used
  auto snapshot = parser->snapshot().offset(parent);

  // should be seeing a leading "." character
  m_assert(parser->advance(), Lexer::Kind::PUNC_PERIOD);

  // attempt gettting the underlying field now
  auto *token = m_label(parser);
  if (token == nullptr) return nullptr;

  // prepare the field identifier
  auto *field = parser->allocate<Syntax::Identifier>(Syntax::Bounds(token), token);

  // prepare the getter to be output when desired now
  auto *getter = parser->allocate<Syntax::Accessor>(snapshot.enclose(token), field, parent);

  // get the current opcode value now
  auto *opcode = parser->current();

  // get the current opcode to be used now
  if (!assignable || !parser->check(Lexer::Flag::ASSIGNS)) return getter;

  // attempt parsing the current assignment expression
  auto *value = (parser->advance(), m_expression(parser));
  if (value == nullptr) return nullptr; // failed to read

  // check the type of opcode to be used now
  switch (opcode->kind()) {
#define SABRE_XX_TOKEN_ASSIGN(T, ...)                                                                                 \
  case Lexer::Kind::ASOP_##T: value = parser->allocate<Syntax::Binary>(Lexer::Kind::BINOP_##T, getter, value); break;
#include "sabre/lexer/_defines/tokens.def"

  // in default situations we break suitably
  default: break;
  }

  // construct the setter to be used now
  return parser->allocate<Syntax::Assign>(snapshot.enclose(opcode), getter, value);
}
