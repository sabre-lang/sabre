/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

/// Type Includes
#include "sabre/type/utility/intrinsics.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Infer, node, compiler, destination) {
  // prepare the trace handler to be used
  $_UNUSED $_AUTO = compiler->trace(node);

  // if the guard is dynamic, then we can return a constant result immediately
  if (node->guard()->trivia()->type()->is<Type::Any>()) {
    if (node->negate()) return compiler->plug<Glyph::LOAD_FALSE>(destination);
    else return compiler->plug<Glyph::LOAD_TRUE>(destination); // always truthy
  }

  auto bx = Register::Accumulator; // prepare ax/bx
  auto ax = compiler->registers()->temporary();

  // lower the left-hand side and the right-hand side
  compiler->lower(node->value(), ax), compiler->lower(node->guard(), bx);

  // force a runtime guard to occur now as necessary
  compiler->plug<Glyph::TYPE_GUARD>(destination, ax, bx);

  // handle the negation of results if necessary
  if (node->negate()) compiler->emit<Glyph::UNOP_NOT>(destination, destination);
}
