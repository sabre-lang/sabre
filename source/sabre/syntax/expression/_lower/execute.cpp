/// Sabre Includes
#include "sabre/bytecode/invoker.hpp"
#include "sabre/bytecode/visitor.hpp"

//  PRIVATE METHODS  //

SABRE_MM_LOWER_NODE(Execute, node, compiler, destination) {
  // prepare the trace to be used now as needed
  $_UNUSED $_AUTO = compiler->trace(node);

  // attempt handling via the underlying invoker
  auto invoker = Bytecode::Invoker(node->callee(), node->policy());
  invoker.compile(compiler, destination, node->arguments());
}
