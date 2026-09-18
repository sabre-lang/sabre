#ifndef _SABRE_FORMAT_LINE_HPP
#define _SABRE_FORMAT_LINE_HPP

/// Sabre Includes
#include "sabre/format/mixin.hpp"

namespace Sabre::Format {

/// @brief Explicit Line Piece.
class Piece::Line : public Piece::Mixin<Line> {
  //  PROPERTIES  //

  /// @brief Denotes the break state.
  Break m_state;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs the line.
   * @param state             Line state.
   */
  explicit Line(Break state) : m_state(state) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the underlying break-state.
  inline constexpr Break state() const noexcept { return m_state; }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles writing a node.
   * @param writer                Format writer.
   * @param wrapping              Current wrapping.
   */
  void m_render(Writer *writer, Wrap wrapping) const noexcept final;

  /**
   * @brief Gets the width of a node.
   * @param wrapped               Wrapped groups.
   * @param force                 Forced width flag.
   */
  size_t m_width(const Wrap::List &wrapped, bool force) const noexcept final;
};

} // namespace Sabre::Format

#endif
