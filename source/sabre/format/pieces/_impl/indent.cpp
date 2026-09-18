/// Sabre Includes
#include "sabre/format/writer.hpp"

/// Format Includes
#include "sabre/format/_inline/pieces.ipp"

//  PRIVATE METHODS  //

void Sabre::Format::Piece::Indent::m_render(Writer *writer, Wrap wrapping) const noexcept {
  // ensure we increment the indentation
  auto marker = writer->indent();

  // write all the nodes as necessary now
  for (const auto *piece : pieces()) piece->render(writer, wrapping);

  // and decrement the final indentation on completion
  writer->dedent(marker);
}
