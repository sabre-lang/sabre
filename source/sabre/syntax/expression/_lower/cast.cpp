/// Sabre Includes
#include "sabre/bytecode/visitor.hpp"

/// Type Includes
#include "sabre/type/utility/intrinsics.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Cast, node, compiler, destination) {
  // prepare the trace handler to be used
  $_UNUSED $_AUTO = compiler->trace(node);

  // if the guard is expected to be dynamic (eg: "Any") then we can ignore it
  auto expected = node->guard()->trivia()->type();
  auto unset = expected->is<Type::Unset>();
  auto dynamic = unset || expected->unify(Type::New::any());
  if (dynamic) return compiler->lower(node->value(), destination);

  auto bx = Register::Accumulator; // ax/bx
  auto ax = compiler->registers()->temporary();

  // lower the left-hand side and the right-hand side
  compiler->lower(node->value(), ax), compiler->lower(node->guard(), bx);

  // force a runtime guard to occur now as necessary
  compiler->plug<Glyph::TYPE_CAST>(destination, ax, bx);
}
