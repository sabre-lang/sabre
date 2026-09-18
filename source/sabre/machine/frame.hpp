#ifndef _SABRE_MACHINE_FRAME_HPP
#define _SABRE_MACHINE_FRAME_HPP

/// Sabre Includes
#include "sabre/engine/frame.hpp"
#include "sabre/function/args.hpp"
#include "sabre/function/environ.hpp"
#include "sabre/machine/info.hpp"

/*
 *
 * The frame-stack for machine-code supports a similar layout to function arguments.
 *
 * |    LABEL    |  OFFSET  |         DESCRIPTION          |
 * | ----------- | -------- | ---------------------------- |
 * |  STK_ENVP   |    0     | The environment context.     |
 * |  STK_ARGV   |    1     | Pointer to frame arguments.  |
 * |  STK_DATA   |    2     | Pointer to callee arguments. |
 *
 */

namespace Sabre::Machine::Offset {

static inline constexpr size_t STK_OPTR = 0;            // Offset pointer.
static inline constexpr size_t STK_ENVP = STK_OPTR + 1; // Environment value.
static inline constexpr size_t STK_PASS = STK_ENVP + 1; // Local arguments.
static inline constexpr size_t STK_DATA = STK_PASS + 1; // Calling arguments.
static inline constexpr size_t STK_SIZE = STK_DATA + 1; // Size of stack.

} // namespace Sabre::Machine::Offset

namespace Sabre::Machine {

/// @brief Derived Machine Frame.
class Frame : public Engine::Frame::Mixin<Frame> {
  //  PROPERTIES  //

  /// @brief Shared function information.
  const Info *m_info;

  /// @brief The underlying constructed stack.
  Value::Any *m_stack = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a function frame.
   * @param isolate               Runtime isolate.
   * @param info                  Function information.
   * @param stack                 Frame stack to bind.
   */
  constexpr Frame(Runtime::Isolate *isolate, const Info *info = nullptr, Value::Any *stack = nullptr) :
      Mixin(isolate), m_info(info), m_stack(stack) {}

  //  PUBLIC METHODS  //

  /// @brief Current function information.
  inline constexpr const Info *info() const noexcept { return m_info; }

  /// @brief Available compilation arena.
  inline constexpr const Image::Arena *arena() const noexcept final { return m_info->arena(); }

  /// @brief Gets the underlying frame resource.
  inline constexpr $::URI::View resource() const noexcept final { return m_info->resource(); }

  /// @brief Gets the current instruction offset.
  inline constexpr uintptr_t offset() const noexcept { return m_stack[Offset::STK_OPTR].pointer(); }

  /// @brief Current stack details.
  inline constexpr Value::Any *&stack() noexcept { return m_stack; }
  inline constexpr Value::Any *stack() const noexcept { return m_stack; }

  /// @brief Handles getting the current self value.
  inline constexpr Value::Any self() const noexcept { return argv()->self(); }

  /// @brief Gets the bound context instance.
  inline constexpr const Function::Environ &context() const noexcept {
    return *reinterpret_cast<Function::Environ *>(&m_stack[Offset::STK_ENVP]);
  }

  /// @brief Casts underlying arguments pointer into an arguments structure.
  inline constexpr const Function::Args *argv() const noexcept {
    return reinterpret_cast<const Function::Args *>(m_stack + Offset::STK_DATA);
  }

  /// @brief Handles constructing suitable backtrace values.
  inline constexpr Resource::Trace backtrace() const noexcept final { return backtrace(m_info->group()); }
  inline constexpr Resource::Trace backtrace(Resource::Group group) const noexcept {
    const auto &binary = arena()->binary; // prepare binary
    auto ptr = offset() + m_info->bytecode().offset();
    auto *entry = binary.records().before(ptr); // resolve
    auto *position = entry ? &entry->position : nullptr;
    return Resource::Trace(resource(), group, position);
  }

  /**
   * @brief Allows iterating over the roots.
   * @param yield               Each callback.
   */
  inline constexpr void roots(Globals::Each &yield) noexcept final {
    yield(m_stack[Offset::STK_ENVP]), yield(m_locals());
  }

private:
  //  PRIVATE METHODS  //

  /// @brief Gets the frames local passthrough values.
  inline constexpr std::span<Value::Any> m_locals() noexcept {
    return {&m_stack[Offset::STK_PASS], m_info->locals() + Function::Offset::ARGS_DATA};
  }
};

} // namespace Sabre::Machine

#endif
