#ifndef _SABRE_MACHINE_ALLOCATOR_HPP
#define _SABRE_MACHINE_ALLOCATOR_HPP

/// Sabre Modules
#include "sabre/forward/machine.hpp"
#include "sabre/register/slot.hpp"

namespace Sabre::Machine {

/// @brief Simple Register Allocator.
class Allocator {
  //  PROPERTIES  //

  /// @brief Underlying compiler instance.
  Compiler *m_compiler = nullptr;

  /// @brief Currently allocated registers.
  $::Map::Base<Register::Encoded, Register::Host> m_registers = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Do not allow default construction.
  explicit Allocator() = default;

  /// @brief Constructs a contiguous register allocator.
  explicit Allocator(Compiler *compiler) : m_compiler(compiler) {}

  //  PUBLIC METHODS  //

  /**
   * @brief Denotes if a register has been bound.
   * @param vreg              Bytecode register.
   */
  inline constexpr bool bound(const Register::Slot &vreg) const noexcept { return m_registers.contains(vreg); }

  /**
   * @brief Resolves an already bound register.
   * @param vreg              Bytecode register.
   */
  inline constexpr Register::Host resolve(const Register::Slot &vreg) const noexcept { return m_registers.at(vreg); }

  /**
   * @brief Handles allocating a register.
   * @param vreg              Bytecode register.
   */
  inline constexpr Register::Host allocate(const Register::Slot &vreg) {
    return allocate(vreg, fmt::format("{0}", vreg));
  }

  /**
   * @brief Handles allocating a register.
   * @param vreg              Bytecode register.
   * @param name              Name of register.
   */
  inline constexpr Register::Host allocate(const Register::Slot &vreg, const $::String::View &name) {
    if (m_registers.contains(vreg)) return m_registers.at(vreg); // get cached value
    return m_registers.emplace(vreg, m_compiler->new_gp64(name.data())).first->second;
  }
};

} // namespace Sabre::Machine

#endif
