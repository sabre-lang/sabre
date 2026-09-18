/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(True, , compiler, destination) { compiler->plug<Glyph::LOAD_TRUE>(destination); }
SABRE_MM_LOWER_NODE(False, , compiler, destination) { compiler->plug<Glyph::LOAD_FALSE>(destination); }
