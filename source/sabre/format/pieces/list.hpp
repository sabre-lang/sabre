#ifndef _SABRE_FORMAT_LIST_HPP
#define _SABRE_FORMAT_LIST_HPP

/// Sabre Includes
#include "sabre/format/mixin.hpp"

namespace Sabre::Format {

/// @brief Explicit List Piece.
class Piece::List : public Piece::Mixin<List> {
  //  PROPERTIES  //

  /// @brief Associated fill pieces.
  Buffer m_pieces = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a list of pieces.
   * @param pieces                List pieces.
   */
  explicit List(const Buffer &pieces = {}) : m_pieces(std::move(pieces)) {}

  /**
   * @brief Constructs a list of pieces.
   * @param capacity              Initial capacity.
   */
  explicit List(size_t capacity) { m_pieces.reserve(capacity); }

  //  PUBLIC METHODS  //

  /// @brief Gets the underlying pieces.
  inline constexpr Buffer &pieces() noexcept { return m_pieces; }
  inline constexpr const Buffer &pieces() const noexcept { return m_pieces; }

  /// @brief Denotes if currently empty.
  inline constexpr bool empty() const noexcept { return m_pieces.empty(); }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles writing a piece.
   * @param writer                Format writer.
   * @param wrapping              Current wrapping.
   */
  virtual void m_render(Writer *writer, Wrap wrapping) const noexcept override;

  /**
   * @brief Gets the width of a piece.
   * @param wrapped               Wrapped groups.
   * @param force                 Forced width flag.
   */
  size_t m_width(const Wrap::List &wrapped, bool force) const noexcept final;
};

} // namespace Sabre::Format

#endif