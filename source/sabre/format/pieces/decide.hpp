#ifndef _SABRE_FORMAT_DECIDE_HPP
#define _SABRE_FORMAT_DECIDE_HPP

/// Sabre Includes
#include "sabre/format/mixin.hpp"

namespace Sabre::Format {

/// @brief Explicit Decision Piece.
class Piece::Decide : public Piece::Mixin<Decide> {
  //  PROPERTIES  //

  /// @brief Bound group sigil.
  Sigil m_group;

  /// @brief Consequence node.
  Piece *m_consequence;

  /// @brief Alternative node.
  Piece *m_alternative;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an anonmyous decision node.
   * @param consequence       Consequence node.
   * @param alternative       Alternative node.
   */
  explicit Decide(Piece *consequence, Piece *alternative = nullptr) : Decide(SIZE_MAX, consequence, alternative) {}

  /**
   * @brief Constructs a decision node.
   * @param group             Group sigil.
   * @param consequence       Consequence node.
   * @param alternative       Alternative node.
   */
  explicit Decide(Sigil group, Piece *consequence, Piece *alternative = nullptr)
      : m_group(group), m_consequence(consequence), m_alternative(alternative) {}

  //  PUBLIC METHODS  //

  inline constexpr Sigil group() const noexcept { return m_group; }
  inline constexpr Piece *consequence() const noexcept { return m_consequence; }
  inline constexpr Piece *alternative() const noexcept { return m_alternative; }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles writing a node.
   * @param writer                Format writer.
   * @param wrapping              Current wrapping.
   */
  virtual void m_render(Writer *writer, Wrap wrapping) const noexcept override;

  /**
   * @brief Gets the width of a node.
   * @param wrapped               Wrapped groups.
   * @param force                 Forced width flag.
   */
  size_t m_width(const Wrap::List &wrapped, bool force) const noexcept final;
};

} // namespace Sabre::Format

#endif