/// Sabre Includes
#include "sabre/format/writer.hpp"

/// Format Includes
#include "sabre/format/_inline/pieces.ipp"

//  PRIVATE METHODS  //

size_t Sabre::Format::Piece::Unicode::m_width(const Wrap::List &, bool) const noexcept { return m_buffer.size(); }
void Sabre::Format::Piece::Unicode::m_render(Writer *writer, Wrap) const noexcept {
  if (!m_quoted) writer->concat(m_buffer);
  else writer->concat("\"", m_buffer, "\"");
}
