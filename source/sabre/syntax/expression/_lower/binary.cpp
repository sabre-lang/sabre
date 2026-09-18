/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"
#include "sabre/string/common.hpp"
#include "sabre/value/fold.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Binary, node, compiler, destination) {
  // pull out the underlying opcode
  auto opcode = node->opcode();

  // get the left/right expressions
  auto *left = node->left(), *right = node->right();
  auto lv = Value::Fold(left), rv = Value::Fold(right);

  // allow returning an immediate value if necessary
  if (auto immediate = Value::Fold(opcode, lv, rv); immediate.pointer().okay()) {
    auto index = compiler->constant(immediate); // prepare constant
    return compiler->plug<Glyph::LOAD_CONST>(destination, index);
  }

  // prepare the bx (or accumulator operand)
  static constexpr auto s_bx = Register::Accumulator;

  // handle any "fast" operations if necessary to do so
  if (rv.is<Number::Tagged>()) {
    switch (opcode) {
#define SABRE_XX_GLYPH_INDEXED(P, N, ...)                                                \
  case Operator::Kind::N:                                                                \
    if (!lv.pointer().okay()) compiler->lower(left, s_bx);                               \
    else compiler->emit<Glyph::LOAD_CONST>(s_bx, compiler->constant(lv));                \
    return compiler->emit<Glyph::P##_##N##I>(destination, s_bx, compiler->constant(rv));
#include "sabre/bytecode/_defines/glyphs.def"
    default: break; // allow handling as normal now
    }
  }

  // prepare the left-most register to be used
  auto ax = compiler->registers()->temporary();

  // compile the left-most expression now as needed
  if (!lv.pointer().okay()) compiler->lower(left, ax);
  else compiler->emit<Glyph::LOAD_CONST>(ax, compiler->constant(lv));

  // and compile the right-most expression now
  if (!rv.pointer().okay()) compiler->lower(right, s_bx);
  else compiler->emit<Glyph::LOAD_CONST>(s_bx, compiler->constant(rv));

  // trace the incoming binary node now
  $_UNUSED $_AUTO = compiler->trace(node);

  // if we have a purely "string" output type, then force a concatenation to occur
  auto concat = left->trivia()->type()->is<String::Any>() && right->trivia()->type()->is<String::Any>();
  if (concat && opcode == Operator::Kind::ADD) return compiler->plug<Glyph::STRING_CONCAT>(destination, ax, s_bx);

  // attempt emitting our necessary opcodes for binary instructions
  switch (opcode) {
#define SABRE_XX_TOKEN_BINARY(N, ...)                                                     \
  case Operator::Kind::N: compiler->emit<Glyph::BINOP_##N>(destination, ax, s_bx); break;
#define SABRE_XX_TOKEN_COMPARE(N, ...)                                                   \
  case Operator::Kind::N: compiler->emit<Glyph::TEST_##N>(destination, ax, s_bx); break;
#include "sabre/lexer/_defines/tokens.def"
  default: $_ABORT("Unknown binary operator '{0}'", Operator::Inspect::name(opcode));
  }
}
