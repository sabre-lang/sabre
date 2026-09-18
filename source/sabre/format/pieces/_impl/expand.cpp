/// Sabre Includes
#include "sabre/format/writer.hpp"

/// Format Includes
#include "sabre/format/_inline/pieces.ipp"

//  PRIVATE METHODS  //

size_t Sabre::Format::Piece::Expand::m_width(const Wrap::List &, bool) const noexcept { return SIZE_MAX; }
void Sabre::Format::Piece::Expand::m_render(Writer *, Wrap) const noexcept {}
