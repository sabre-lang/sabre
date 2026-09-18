/// Sabre Includes
#include "sabre/format/writer.hpp"

/// Format Includes
#include "sabre/format/_inline/pieces.ipp"

//  PRIVATE METHODS  //

size_t Sabre::Format::Piece::Decide::m_width(const Wrap::List &wrapped, bool force) const noexcept {
  auto wrapping = m_group != SIZE_MAX && wrapped.contains(m_group);
  if (wrapping) return m_consequence->width(wrapped, force);
  return m_alternative ? m_alternative->width(wrapped, force) : 0;
}

void Sabre::Format::Piece::Decide::m_render(Writer *writer, Wrap wrapping) const noexcept {
  // prepare the condition to be decided between (for anonymous or explicit)
  auto condition = (m_group == SIZE_MAX && wrapping.enabled()) || writer->wrapped().contains(m_group);

  // attempt running our desired condition now
  if (condition) m_consequence->render(writer, Wrap::State::ENABLED);
  else if (m_alternative) m_alternative->render(writer, wrapping);
}
