/// Sabre Includes
#include "sabre/format/writer.hpp"

/// Format Includes
#include "sabre/format/_inline/pieces.ipp"

//  PRIVATE METHODS  //

size_t Sabre::Format::Piece::Space::m_width(const Wrap::List &, bool) const noexcept { return 1; }
void Sabre::Format::Piece::Space::m_render(Writer *writer, Wrap wrapping) const noexcept {
  if (m_state == Break::HARD || !wrapping.enabled()) writer->space();
}
