#ifndef _SABRE_FORMAT_MINIMIZE_HPP
#define _SABRE_FORMAT_MINIMIZE_HPP

/// Sabre Includes
#include "sabre/format/mixin.hpp"

namespace Sabre::Format {

/// @brief Explicit Zero-Width Piece.
class Piece::Minimize : public Piece::Mixin<Minimize> {
  //  PROPERTIES  //

  /// @brief Minimization target.
  Piece *m_target = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Compresses a node to zero-width.
  explicit Minimize() = default;

  /**
   * @brief Constructs a minimization node.
   * @param target            Target to minimize.
   */
  explicit Minimize(Piece *target) : m_target(target) {}

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