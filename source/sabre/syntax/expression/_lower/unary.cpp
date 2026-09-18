/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"
#include "sabre/value/fold.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Unary, node, compiler, destination) {
  // attempt resolving an immediate value if possible
  if (auto immediate = Value::Fold(node); immediate.pointer().okay()) {
    auto constant = compiler->constant(immediate); // pre-build now
    return compiler->emit<Glyph::LOAD_CONST>(destination, constant);
  }

  // emit the operand to begin with onto the sink
  compiler->lower(node->operand(), Register::Accumulator);

  // trace the incoming binary node now
  $_UNUSED $_AUTO = compiler->trace(node);

  // attempt emitting based on the incoming opcodes
  switch (node->opcode()) {
#define SABRE_XX_TOKEN_UNARY(N, ...)                                                                  \
  case Operator::Kind::N: compiler->plug<Glyph::UNOP_##N>(destination, Register::Accumulator); break;
    SABRE_XX_TOKEN_UNARY(NEG)
#include "sabre/lexer/_defines/tokens.def"
  default: $_ABORT("Unknown unary operator '{0}'", Operator::Inspect::name(node->opcode()));
  }
}
