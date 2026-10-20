/// Sabre Includes
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
  auto bx = Register::Accumulator;

  // handle any "fast" operations if necessary to do so
  if (rv.is<Number::Tagged>()) {
    switch (opcode) {
#define SABRE_XX_GLYPH_INDEXED(P, N, ...)                                              \
  case Operator::Kind::N:                                                              \
    if (!lv.pointer().okay()) bx = compiler->reduce(left, bx);                         \
    else compiler->emit<Glyph::LOAD_CONST>(bx, compiler->constant(lv));                \
    return compiler->emit<Glyph::P##_##N##I>(destination, bx, compiler->constant(rv));
#include "sabre/bytecode/_defines/glyphs.def"
    default: break; // allow handling as normal now
    }
  }

  // prepare the left-most register to be used
  auto scoped = compiler->registers()->temporary();
  Register::Slot ax = scoped; // copy a reference

  // compile the left-most expression now as needed
  if (!lv.pointer().okay()) ax = compiler->reduce(left, ax);
  else compiler->emit<Glyph::LOAD_CONST>(ax, compiler->constant(lv));

  // and compile the right-most expression now
  if (!rv.pointer().okay()) bx = compiler->reduce(right, bx);
  else compiler->emit<Glyph::LOAD_CONST>(bx, compiler->constant(rv));

  // trace the incoming binary node now
  $_UNUSED $_AUTO = compiler->trace(node);

  // if we have a purely "string" output type, then force a concatenation to occur
  auto concat = left->trivia()->type()->is<String::Any>() && right->trivia()->type()->is<String::Any>();
  if (concat && opcode == Operator::Kind::ADD) return compiler->plug<Glyph::STRING_CONCAT>(destination, ax, bx);

  // attempt emitting our necessary opcodes for binary instructions
  switch (opcode) {
#define SABRE_XX_TOKEN_BINARY(N, ...)                                                   \
  case Operator::Kind::N: compiler->emit<Glyph::BINOP_##N>(destination, ax, bx); break;
#define SABRE_XX_TOKEN_COMPARE(N, ...)                                                 \
  case Operator::Kind::N: compiler->emit<Glyph::TEST_##N>(destination, ax, bx); break;
#include "sabre/lexer/_defines/tokens.def"
  default: $_ABORT("Unknown binary operator '{0}'", Operator::Inspect::name(opcode));
  }
}
