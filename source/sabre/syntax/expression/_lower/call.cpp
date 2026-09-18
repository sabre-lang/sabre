/// Sabre Modules
#include "sabre/bytecode/invoker.hpp"
#include "sabre/bytecode/visitor.hpp"

//  PRIVATE METHODS  //

SABRE_MM_LOWER_NODE(Call, node, compiler, destination) {
  $_UNUSED $_AUTO = compiler->trace(node); // prepare trace
  auto invoker = Bytecode::Invoker(node->callee(), false);
  invoker.compile(compiler, destination, node->arguments());
}
