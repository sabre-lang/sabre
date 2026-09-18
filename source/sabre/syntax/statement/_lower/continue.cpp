/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Continue, , compiler, ) { compiler->emit<Glyph::JUMP_TO>(compiler->labels()->continues()); }
