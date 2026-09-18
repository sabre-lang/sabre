/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Void, , compiler, destination) { compiler->plug<Glyph::LOAD_VOID>(destination); }
