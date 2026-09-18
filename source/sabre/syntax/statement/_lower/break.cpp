/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Break, , compiler, ) { compiler->emit<Glyph::JUMP_TO>(compiler->labels()->breaks()); }
