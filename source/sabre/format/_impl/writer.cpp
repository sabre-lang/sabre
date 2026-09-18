/// Sabre Include
#include "sabre/format/writer.hpp"

//  CONSTRUCTORS  //

Sabre::Format::Writer::Writer() : Writer($::Global::get<Format::Options>()) {}
Sabre::Format::Writer::Writer(const Options *options) : m_options(options) {}
