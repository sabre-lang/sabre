/// Sabre Includes
#include "sabre/format/writer.hpp"

/// Format Includes
#include "sabre/format/_inline/pieces.ipp"

//  PRIVATE METHODS  //

size_t Sabre::Format::Piece::Line::m_width(const Wrap::List &, bool) const noexcept {
  switch (m_state) {
  case Break::SOFT: return 0;
  case Break::DYNAMIC: return 1;
  default: return SIZE_MAX;
  }
}

void Sabre::Format::Piece::Line::m_render(Writer *writer, Wrap wrapping) const noexcept {
  // check if the writer is currently empty
  if (writer->empty()) return;

  // write the line as necessary now
  switch (m_state) {
  case Break::HARD: writer->newline(); break;
  case Break::LITERAL: writer->newline(0); break;
  case Break::SOFT: wrapping.enabled() ? writer->newline() : $_PP_NOOP(); break;
  case Break::DYNAMIC: wrapping.enabled() ? writer->newline() : writer->space(); break;
  default: $_ABORT("Unknown line-break found"); // declare as an invalid incoming state
  }
}
