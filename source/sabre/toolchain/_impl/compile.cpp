/// Sabre Modules
#include "sabre/toolchain/compile.hpp"
#include "sabre/bytecode/compiler.hpp"

//  PUBLIC METHODS  //

$::Unique::Pointer<Sabre::Image::Arena> Sabre::Toolchain::compile(const Syntax::Tree *syntax, XI::Container *services) {
  return services->get<Bytecode::Compiler>()->process(syntax);
}
