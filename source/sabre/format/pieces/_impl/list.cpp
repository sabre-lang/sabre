/// Sabre Includes
#include "sabre/format/writer.hpp"

/// Format Includes
#include "sabre/format/_inline/pieces.ipp"

//  PRIVATE METHODS  //

size_t Sabre::Format::Piece::List::m_width(const Wrap::List &wrapped, bool force) const noexcept {
  return m_apply(m_pieces, wrapped, force);
}

void Sabre::Format::Piece::List::m_render(Writer *writer, Wrap wrapping) const noexcept {
  for (const auto &piece : m_pieces) piece->render(writer, wrapping);
}
