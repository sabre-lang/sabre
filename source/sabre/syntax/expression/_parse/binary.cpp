/// Syntax Includes
#include "sabre/syntax/_inline/expression.ipp"
#include "sabre/syntax/_inline/generator.ipp"

//  PRIVATE METHODS   //

SABRE_MM_PARSE_INFIX(Binary, parser, left, ) {
  // prepare the snapshot to be used
  auto snapshot = parser->snapshot().offset(left);

  // cache the operator now
  auto opcode = parser->current();

  // prepare the grammar to be inherited
  static auto s_grammar = Syntax::Generator::grammar<Syntax::Expression, Stream, Dispatch>();

  // ensure we have a valid opcode to be used now
  switch (opcode->kind()) {
#define SABRE_XX_TOKEN_BINARY(N, ...)                    \
  case Lexer::Kind::BINOP_##N: parser->advance(); break;
#include "sabre/lexer/_defines/tokens.def"
  default: return parser->report(opcode, 2000100, "a binary operator");
  }

  // attempt getting the suitable rule to be used now
  auto rule = s_grammar.at(static_cast<size_t>(opcode->kind()));
  auto level = static_cast<size_t>(rule.rank) + 1;

  // parse the incoming RHS of the binary operation
  auto right = m_expression(parser, static_cast<Precedence>(level));
  if (!right) return nullptr; // invalid RHS found

  // and return the resulting binary node now
  return parser->allocate<Syntax::Binary>(snapshot.enclose(opcode), opcode, left, right);
}
