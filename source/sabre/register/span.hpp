#ifndef _SABRE_REGISTER_SPAN_HPP
#define _SABRE_REGISTER_SPAN_HPP

/// Sabre Include
#include "sabre/register/slot.hpp"

namespace Sabre::Register {

/// @brief Register List View.
struct Span : public Engine::Operand<3> {
  //  TYPEDEFS  //

  /// @brief The encoded list typing.
  using Encoded = uint32_t;

protected:
  //  PRIVATE METHODS  //

  /// @brief Total register count.
  uint8_t m_count = 0;

  /// @brief The first register attached.
  Register::Encoded m_first = Slot::m_maximum();

  static constexpr Encoded m_shift = 16;    // Shift constant.
  static constexpr Encoded m_mask = 0xFFFF; // Mask constant.

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a defaulted span.
  constexpr Span() = default;

  /**
   * @brief Constructs a span from an encoded value.
   * @param encoded               Encoded span value.
   */
  constexpr Span(Encoded encoded) : Span(encoded & m_mask, encoded >> m_shift) {}

  /**
   * @brief Constructs a span.
   * @param first                 Initial value.
   * @param count                 Total span count.
   */
  constexpr Span(Register::Encoded first, uint8_t count) : m_count(count), m_first(first) {}

  //  PUBLIC METHODS  //

  /// @brief Checks if the register span is empty.
  inline constexpr bool empty() const noexcept { return m_count == 0; }

  /// @brief Gets the size of the register span.
  inline constexpr uint8_t count() const noexcept { return m_count; }

  /// @brief Gets the first-most register value.
  inline constexpr Slot first() const noexcept { return m_count ? m_first : UINT16_MAX; }

  /// @brief Gets the last-most register value.
  inline constexpr Slot last() const noexcept { return m_count ? m_first + (m_count - 1) : UINT16_MAX; }

  /// @brief Handles encoding register spans.
  inline constexpr Encoded encode() const noexcept { return m_first | (m_count << m_shift); }

  /**
   * @brief Handles slicing spans.
   * @param offset                Leading offset.
   */
  inline constexpr Span slice(uint8_t offset = 0) const noexcept {
    if (empty() || offset > m_count) return Span();
    return Span(m_first + offset, m_count - offset);
  }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Prints a register span.
   * @param os                    Output stream.
   * @param self                  Register span.
   */
  static inline void m_print(std::ostream &os, const Span &self) {
    if (self.m_count == 0) os << "()"; // empty
    else os << self.first() << '-' << self.last();
  }
};

/// @brief Register List Value.
class List : public Span {
  //  PROPERTIES  //

  /// @brief Underlying allocator reference.
  Bytecode::Allocator *m_allocator;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a register-list.
   * @param allocator             Register allocator.
   */
  List(Bytecode::Allocator *allocator = nullptr);

  /// @brief Ensures lists are freed on deletion.
  ~List();

  //  PUBLIC METHODS  //

  /// @brief Grows the list instance.
  Slot grow();
};

} // namespace Sabre::Register

#endif
