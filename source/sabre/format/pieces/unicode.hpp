#ifndef _SABRE_FORMAT_UNICODE_HPP
#define _SABRE_FORMAT_UNICODE_HPP

/// Sabre Includes
#include "sabre/format/mixin.hpp"

namespace Sabre::Format {

/// @brief Explicit Text Piece.
class Piece::Unicode : public Piece::Mixin<Unicode> {
  //  PROPERTIES  //

  /// @brief Denotes if should be quoted.
  bool m_quoted;

  /// @brief Associated text buffer.
  $::String::Buffer m_buffer;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a text node.
   * @param buffer                Text buffer.
   * @param quoted                Quoted state.
   */
  explicit Unicode(const $::String::View &buffer = "", bool quoted = false) : m_quoted(quoted), m_buffer(buffer) {}

  //  PUBLIC METHODS  //

  /// @brief Denotes if the buffer should be quoted.
  inline constexpr bool quoted() const { return m_quoted; }

  /// @brief Gets a view of the underlying unicode buffer.
  inline constexpr $::String::View view() const { return m_buffer; }

  /// @brief Gets a view of the underlying unicode buffer.
  inline constexpr const $::String::Buffer &buffer() const { return m_buffer; }

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
