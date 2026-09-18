#ifndef _SABRE_REGISTER_SLOT_HPP
#define _SABRE_REGISTER_SLOT_HPP

/// Sabre Include
#include "sabre/engine/operand.hpp"
#include "sabre/forward/register.hpp"

namespace Sabre::Register {

/// @brief Register Slot Typing.
struct Slot : public Engine::Operand<2>, public $::Unit::Strong<Encoded, Slot> {
  //  TYPEDEFS  //

  /// @brief Allow spans internal access.
  friend struct Span;

  //  CONSTRUCTORS  //

  /// @brief Inherit the baseline constructors.
  using Strong::Strong;

  /// @brief Constructs an empty register value.
  constexpr Slot() : Strong(m_maximum()) {}

  //  PUBLIC METHODS  //

  /// @brief Handles encoding the register.
  inline constexpr Encoded encode() const noexcept { return m_value; }

  /// @brief Denotes if the register is to "nowhere".
  inline constexpr bool nowhere() const noexcept { return m_value == m_maximum(); }

  /// @brief Checks if the register is an accumulator.
  inline constexpr bool accumulator() const noexcept { return m_value == m_minimum(); }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles printing register values.
   * @param os                    Output stream.
   * @param self                  Register instance.
   */
  static inline void m_print(std::ostream &os, const Slot &self) {
    if (self.nowhere()) os << "RN";
    else if (self.accumulator()) os << "RA";
    else os << 'R' << self.m_value;
  }
};

/// @brief Define an accumulator slot.
static inline constexpr Slot Accumulator = 0;

} // namespace Sabre::Register

#endif
