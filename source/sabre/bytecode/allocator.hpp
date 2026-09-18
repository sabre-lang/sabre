#ifndef _SABRE_BYTECODE_ALLOCATOR_HPP
#define _SABRE_BYTECODE_ALLOCATOR_HPP

/// Sabre Includes
#include "sabre/register/scoped.hpp"
#include "sabre/register/span.hpp"

namespace Sabre::Bytecode {

/// @brief Simple Contiguous Register Alloctor.
class Allocator {
  //  TYPEDEFS  //

  /// @brief Allow registers internal access.
  friend class Register::List;
  friend class Register::Scoped;

  //  PROPERTIES  //

  Register::Encoded m_index = 1; // Next register index.
  Register::Encoded m_count = 1; // Current maximum count.

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a simple register allocator.
  constexpr Allocator() = default;

  //  PUBLIC METHODS  //

  /// @Brief Gets a consist accumulator address.
  inline constexpr const Register::Slot &accumulator() const noexcept { return Register::Accumulator; }

  /// @brief Constructs a new register list.
  inline constexpr Register::List list() { return Register::List(this); }

  /// @brief Constructs a temporary register value.
  inline constexpr Register::Scoped temporary() { return Register::Scoped(this); }

  /// @brief Handles resetting the allocator instance.
  inline constexpr Allocator &reset() { return m_index = m_count = 1, *this; }

  /// @brief Gets the current maximum register value.
  inline constexpr Register::Encoded maximum() const noexcept { return m_count; }

  /// @brief Handles allocating a new register value.
  inline constexpr Register::Slot allocate() { return m_count = std::max(++m_index, m_count), m_index - 1; }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Releases all registers above an index.
   * @param index                     Register index to delist.
   */
  inline void m_delist(Register::Encoded index) { m_index = index; }

  /**
   * @brief Releases a singular register.
   * @param reg                       Register to release.
   */
  inline void m_release($_UNUSED Register::Slot reg) {
    $_ASSERT(m_index - 1 == reg, "Register release mismatch"), m_index--;
  }
};

} // namespace Sabre::Bytecode

#endif
