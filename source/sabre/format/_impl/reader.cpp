/// Sabre Include
#include "sabre/format/reader.hpp"

//  CONSTRUCTORS  //

Sabre::Format::Reader::Reader(const Lexer::Buffer *tokens) : Reader(tokens, $::Global::get<Format::Options>()) {}
Sabre::Format::Reader::Reader(const Lexer::Buffer *tokens, const Options *options) :
    Visitor(tokens), m_options(options) {}
