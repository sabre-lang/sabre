/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"
#include "sabre/function/common.hpp"

/// Builtin Modules
#include "sabre/builtins/_inline/builtins.ipp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Signature, , compiler, destination) {
  static auto s_name = Value::Symbol(Value::Inspect<Function::Any>::name()); // prepare the name to be used
  if (!destination.nowhere()) compiler->emit<Glyph::LOAD_GLOBAL>(destination, compiler->constant(s_name));
}
