#ifndef _SABRE_FORMAT_INDENT_HPP
#define _SABRE_FORMAT_INDENT_HPP

/// Format Includes
#include "sabre/format/pieces/list.hpp"

namespace Sabre::Format {

/// @brief Explicit Indent Piece.
struct Piece::Indent : public Piece::Mixin<Indent, List> {
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Mixin::Mixin;

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles writing a node.
   * @param writer                Format writer.
   * @param wrapping              Current wrapping.
   */
  void m_render(Writer *writer, Wrap wrapping) const noexcept final;
};

} // namespace Sabre::Format

#endif