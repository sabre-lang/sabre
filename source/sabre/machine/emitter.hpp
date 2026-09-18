#ifndef _SABRE_MACHINE_EMITTER_HPP
#define _SABRE_MACHINE_EMITTER_HPP

/// Sabre Modules
#include "sabre/engine/interrupt.hpp"
#include "sabre/machine/constants.hpp"
#include "sabre/register/slot.hpp"

namespace Sabre::Machine {

/// @brief Denotes Validation Checks.
enum class Validate : bool { SLOW = false, FAST = true };

/// @brief Machine Code Emitter.
class Emitter {
  //  TYPEDEFS  //

  /// @brief Alias the callee typing.
  using Callee = XASM::Function::Callee;

  //  PROPERTIES  //

  /// @brief The current bytecode offset.
  size_t m_offset = 0;

  /// @brief Underlying builder instance.
  Builder *m_builder = nullptr;

  /// @brief Underlying compiler instance.
  Compiler *m_compiler = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Do not allow default construction.
  explicit Emitter() = default;

  /**
   * @brief Constructs an emitter instance.
   * @param builder           Builder to bind.
   */
  explicit Emitter(Builder *builder);

  /**
   * @brief Resolve a label relative to a jump.
   * @param index             Jump index.
   */
  Label label(const Bytecode::Index &index);

  /**
   * @brief Handles binding an instruction header.
   * @param instruction       Bytecode instruction.
   * @param offset            Offset of bytecode.
   */
  void header(const Bytecode::Instruction *instruction, size_t offset);

  /**
   * @brief Converts a register slot to host.
   * @param vreg              Virtual register.
   */
  Register::Host slot(const Register::Slot &vreg);

  /**
   * @brief Constructs immediates from values.
   * @param value             Value to cast.
   */
  Immediate imm(const Value::Any &value) const noexcept;

  /**
   * @brief Constructs immediates from references.
   * @param value             Value to cast.
   */
  Immediate ref(const Value::Any &value) const noexcept;

  /**
   * @brief Constructs memory pointers.
   * @param vreg             Host register.
   * @param offset           Alignment offset.
   */
  Memory mem(const Register::Host &vreg, size_t offset = 0) const noexcept;

  /**
   * @brief Handles loading the self value.
   * @param vreg              Host register.
   */
  void self(const Register::Slot &vreg);
  void self(const Register::Host &vreg);

  /**
   * @brief Handles loading values into a register.
   * @param vreg              Host register.
   * @param value             Value to load.
   */
  void load(const Value::Any &value);
  void load(const Register::Slot &vreg, const Value::Any &value);
  void load(const Register::Host &vreg, const Value::Any &value);
  void load(const Register::Slot &vreg, const Immediate &value);
  void load(const Register::Host &vreg, const Immediate &value);
  void load(const Bytecode::Instruction *unqualified, uint16_t depth = 0);

  /**
   * @brief Handles moving registers.
   * @param dst               Sink register.
   * @param src               Source register.
   */
  void move(const Register::Slot &dst, const Register::Slot &src);
  void move(const Register::Slot &dst, const Register::Host &src);
  void move(const Register::Host &dst, const Register::Slot &src);
  void move(const Register::Host &dst, const Register::Host &src);
  void move(const Bytecode::Instruction *unqualified, uint16_t depth = 0);

  /**
   * @brief Swaps two registers.
   * @param ax                Left register.
   * @param bx                Right register.
   */
  void swap(const Register::Slot &ax, const Register::Slot &bx);
  void swap(const Register::Slot &ax, const Register::Host &bx);
  void swap(const Register::Host &ax, const Register::Slot &bx);
  void swap(const Register::Host &ax, const Register::Host &bx);

  /**
   * @brief Handles setting up parameters.
   * @param self               Self register.
   * @param span               Arguments span.
   */
  void params(const Register::Span &span = {});
  void params(const Register::Slot &self, const Register::Span &span = {});

  /**
   * @brief Handles emitting getters.
   * @param dx                Destination register.
   * @param tx                Target register.
   * @param index             Index of constant.
   */
  void getter(const Register::Host &dx, const Register::Host &tx, const Bytecode::Index &index);

  /**
   * @brief Handles emitting getters.
   * @param tx                Target register.
   * @param vx                Value register.
   * @param index             Index of constant.
   */
  void setter(const Register::Host &tx, const Register::Host &vx, const Bytecode::Index &index);

  /**
   * @brief Handles converting values to floats.
   * @param label             Bailout label.
   * @param xmm               Float register.
   * @param vreg              Value register.
   */
  void vtof(const Label &label, const Double &xmm, const Register::Host &vreg);

  /**
   * @brief Handles converting floats to values.
   * @param xmm                Float register.
   * @param vreg               Value register.
   */
  void ftov(const Double &xmm, const Register::Host &vreg);

  /**
   * @brief Emits a required operation.
   * @param instruction       Instruction to compile.
   */
  void unary(const Bytecode::Instruction *instruction);
  void binary(const Bytecode::Instruction *instruction);
  void negate(const Bytecode::Instruction *instruction);
  void logical(const Bytecode::Instruction *instruction);
  void coalesce(const Bytecode::Instruction *instruction);
  void equality(const Bytecode::Instruction *instruction);
  void compare(const Bytecode::Instruction *instruction);

  /**
   * @brief Returns from a given register.
   * @param vreg              Host register.
   * @param value             Value to return.
   */
  void ret();
  void ret(const Value::Any &value);
  void ret(const Register::Host &vreg);
  void ret(const Register::Host &vreg, const Value::Any &value);

  /**
   * @brief Coordinates an interrupt request.
   * @param mode              Optional mode check.
   */
  void irq();
  void irq(Engine::Interrupt mode);

  /**
   * @brief Validates register values.
   * @param vreg              Register to validate.
   * @param mode              Mode to coordinate.
   */
  void test(const Register::Slot &vreg, Validate mode);
  void test(const Register::Host &vreg, Validate mode);

  /**
   * @brief Coordinates truthiness jumps.
   * @param label             Label to jump.
   * @param vreg              Condition register.
   */
  void jmpt(const Label &label, const Register::Slot &vreg);
  void jmpt(const Label &label, const Register::Host &vreg);

  /**
   * @brief Coordinates falsiness jumps.
   * @param label             Label to jump.
   * @param vreg              Condition register.
   */
  void jmpf(const Label &label, const Register::Slot &vreg);
  void jmpf(const Label &label, const Register::Host &vreg);

  /**
   * @brief Coordinates constant jumps.
   * @param label             Label to jump.
   * @param vreg              Condition register.
   * @param value             Constant value.
   */
  void jmpc(const Label &label, const Register::Slot &vreg, const Value::Any &value);
  void jmpc(const Label &label, const Register::Host &vreg, const Value::Any &value);
  void jmpc(const Label &label, const Register::Slot &vreg, const Bytecode::Index &index);
  void jmpc(const Label &label, const Register::Host &vreg, const Bytecode::Index &index);

  /**
   * @brief Handles invoking given functions.
   * @param glue              Callback to bind.
   * @param args              Arguments to bind.
   */
  template <class F, class... As> inline constexpr Callee *call(F glue, As &&...args) {
    return XASM::Function::Invoke(m_compiler, glue, std::forward<As>(args)...);
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles loading arguments to a register.
   * @param vreg              Virtual register.
   * @param offset            Argument offset.
   */
  void m_argv(const Register::Host &vreg, size_t offset = 0);
};

} // namespace Sabre::Machine

#endif
