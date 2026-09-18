#ifndef _SABRE_FORMAT_GROUP_HPP
#define _SABRE_FORMAT_GROUP_HPP

/// Format Includes
#include "sabre/format/pieces/list.hpp"

namespace Sabre::Format {

/// @brief Explicit Group Piece.
class Piece::Group : public Piece::Mixin<Group, List> {
  //  PROPERTIES  //

  /// @brief The associated group sigil.
  Sigil m_sigil = 0;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a baseline group.
   * @param sigil                 Sigil to bind.
   * @param capacity              Initial capacity.
   */
  explicit Group(Sigil sigil, size_t capacity) : Mixin(capacity), m_sigil(sigil) {}

  /**
   * @brief Constructs a baseline group.
   * @param sigil                 Sigil to bind.
   * @param nodes                 Group nodes.
   */
  explicit Group(Sigil sigil, const Buffer &nodes = {}) : Mixin(nodes), m_sigil(sigil) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the underlying group sigil.
  inline constexpr Sigil sigil() const noexcept { return m_sigil; }

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