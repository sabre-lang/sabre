#ifndef _SABRE_FORMAT_SPACE_HPP
#define _SABRE_FORMAT_SPACE_HPP

/// Sabre Includes
#include "sabre/format/mixin.hpp"

namespace Sabre::Format {

/// @brief Explicit Space Piece.
class Piece::Space : public Piece::Mixin<Space> {
  //  PROPERTIES  //

  /// @brief Denotes the break state.
  Break m_state = Break::SOFT;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs the space.
   * @param state             Space state.
   */
  explicit Space(Break state) : m_state(state) {}

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
