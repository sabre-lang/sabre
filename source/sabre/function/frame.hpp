#ifndef _SABRE_FUNCTION_FRAME_HPP
#define _SABRE_FUNCTION_FRAME_HPP

/// Sabre Includes
#include "sabre/engine/frame.hpp"
#include "sabre/function/args.hpp"
#include "sabre/function/environ.hpp"
#include "sabre/function/info.hpp"

namespace Sabre::Function {

/// @brief Argument Split Typing.
using Split = std::pair<Value::Any, std::span<Value::Any>>;

/// @brief Derived Function Frame.
class Frame : public Engine::Frame::Mixin<Frame> {
  //  PROPERTIES  //

  /// @brief Shared function information.
  const Info *m_info = nullptr;

  /// @brief Frame stack values.
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

  /// @brief Handles getting the internal offset value.
  inline constexpr uintptr_t &offset() noexcept { return *reinterpret_cast<uintptr_t *>(&m_stack[Offset::STK_OPTR]); }
  inline constexpr uintptr_t offset() const noexcept { return m_stack[Offset::STK_OPTR].pointer(); }

  /// @brief Handles jumping to the next instruction.
  inline constexpr uintptr_t advance() noexcept { return (offset() += Bytecode::Width) - Bytecode::Width; }

  /// @brief Current stack details.
  inline constexpr Value::Any *stack() const noexcept { return m_stack; }

  /// @brief Gets the underlying arguments count.
  inline constexpr Value::Any self() const noexcept { return m_stack[Offset::STK_SELF]; }

  /// @brief Gets the available arguments for the frame.
  inline constexpr std::span<Value::Any> argv() const noexcept { return span(1, m_argc()); }

  /// @brief Gets the bound context instance.
  inline constexpr Environ &context() noexcept { return *reinterpret_cast<Environ *>(&m_stack[Offset::STK_ENVP]); }
  inline constexpr const Environ &context() const noexcept {
    return *reinterpret_cast<Environ *>(&m_stack[Offset::STK_ENVP]);
  }

  /// @brief Gets the accumulator value to be used.
  inline constexpr Value::Any &accumulator() noexcept { return m_stack[Offset::STK_ACCU]; }
  inline constexpr const Value::Any &accumulator() const noexcept { return m_stack[Offset::STK_ACCU]; }

  /**
   * @brief Handles getting registers from the frame.
   * @param index                 Register index.
   */
  $_INLINE_PERF constexpr Value::Any load(const Register::Slot &index) const { return m_assert(index), m_stack[index]; }

  /**
   * @brief Handles getting registers from the frame.
   * @param index                 Register index.
   */
  template <std::derived_from<Value::Any> T> $_INLINE_PERF constexpr T load(const Register::Slot &index) const {
    return load(index).as<T>();
  }

  /**
   * @brief Handles setting registers from the frame.
   * @param index                 Register index.
   * @param value                 Value to assign.
   */
  $_INLINE_PERF constexpr Value::Any store(const Register::Slot &index, const Value::Any &value) {
    return m_assert(index), m_stack[index] = value;
  }

  /**
   * @brief Handles swapping two stack items.
   * @param ax                    Alpha register.
   * @param bx                    Beta register.
   */
  inline constexpr void swap(const Register::Slot &ax, const Register::Slot &bx) noexcept {
    std::swap(m_stack[ax], m_stack[bx]);
  }

  /**
   * @brief Handles getting a frame span.
   * @param list                  Register list.
   */
  inline constexpr std::span<Value::Any> span(const Register::Span &list) const {
    return span(list.first(), list.count());
  }

  /**
   * @brief Handles getting a frame span.
   * @param index                 Frame index.
   * @param count                 Span count.
   */
  inline constexpr std::span<Value::Any> span(const Register::Slot &index, Bytecode::Index count) const {
    if (count == 0) return std::span<Value::Any>(); // resolve empty spans immediately
    return m_assert(index + count - 1), std::span<Value::Any>({&m_stack[index], count});
  }

  /**
   * @brief Handles constructing call-arguments.
   * @param list                  List to split.
   */
  inline constexpr Split split(const Register::Span &list) const { return split(span(list)); }
  inline constexpr Split split(const std::span<Value::Any> &span) const { return {span[0], span.subspan(1)}; }

  /**
   * @brief Retrieves a string intern.
   * @param index                 Index of intern.
   */
  inline constexpr const String::Intern &intern(Bytecode::Index index) const noexcept {
    return arena()->strings.at(index);
  }

  /**
   * @brief Retrieves a constant.
   * @tparam T                    Optional type.
   * @param index                 Index of constant.
   */
  template <std::derived_from<Value::Any> T = Value::Any>
  inline constexpr T constant(Bytecode::Index index) const noexcept {
    auto value = arena()->constants.at(index); // getter
    if constexpr (std::same_as<T, Value::Any>) return value;
    else return value.as<T>(); // force a cast to occur
  }

  /// @brief Handles constructing suitable backtrace values.
  inline constexpr Resource::Trace backtrace() const noexcept final { return backtrace(m_info->group()); }
  inline constexpr Resource::Trace backtrace(Resource::Group group) const noexcept {
    const auto &binary = arena()->binary; // prepare binary
    auto ptr = offset() - binary.address() - Bytecode::Width;
    auto *entry = binary.records().before(ptr); // resolve
    auto *position = entry ? &entry->position : nullptr;
    return Resource::Trace(resource(), group, position);
  }

  /**
   * @brief Allows iterating over the roots.
   * @param yield               Each callback.
   */
  inline constexpr void roots(Globals::Each &yield) noexcept final {
    yield(context().environment());
    yield(span(0, info()->locals()));
  }

private:
  //  PRIVATE METHODS  //

  /// @brief Gets the argument count for this frame.
  inline constexpr size_t m_argc() const noexcept { return m_stack[Offset::STK_SIZE].pointer(); }

  /**
   * @brief Ensures indices are valid.
   * @param index                 Index to validate.
   */
  inline constexpr void m_assert($_UNUSED const Register::Slot &index) const {
    $_ASSERT(!index.nowhere(), "Invalid register value"); // validate now
    $_ASSERT(index.encode() < m_info->locals() + 1, "Exceeded stack index");
  }
};

} // namespace Sabre::Function

#endif
