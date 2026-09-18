/// Syntax Includes
#include "sabre/syntax/_inline/expression.ipp"

//  PRIVATE METHODS   //

SABRE_MM_PARSE_PREFIX(Unary, parser, ) {
  // prepare the snapshot to be used
  auto snapshot = parser->snapshot();

  // cache the operator now
  auto opcode = parser->current();

  // ensure we have a valid opcode to be used now
  switch (opcode->kind()) {
#define SABRE_XX_TOKEN_UNARY(N, ...)                    \
  case Lexer::Kind::UNOP_##N: parser->advance(); break;
#include "sabre/lexer/_defines/tokens.def"
  case Lexer::Kind::BINOP_SUB: parser->advance(); break;
  default: return parser->report(2000100, "a unary operator");
  }

  // ensure we have a valid node to be used
  auto operand = m_expression(parser, Precedence::UNARY);
  if (!operand) return nullptr; // ignore since invalid

  // and construct the unary expression to be bound
  return parser->allocate<Syntax::Unary>(snapshot.enclose(opcode), opcode->kind(), operand);
}
