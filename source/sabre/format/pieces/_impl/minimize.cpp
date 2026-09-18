/// Sabre Includes
#include "sabre/format/writer.hpp"

/// Format Includes
#include "sabre/format/_inline/pieces.ipp"

//  PRIVATE METHODS  //

size_t Sabre::Format::Piece::Minimize::m_width(const Wrap::List &wrapped, bool force) const noexcept {
  return force && m_target ? m_target->width(wrapped, force) : 0;
}

void Sabre::Format::Piece::Minimize::m_render(Writer *writer, Wrap wrapping) const noexcept {
  if (m_target) m_target->render(writer, wrapping);
}
