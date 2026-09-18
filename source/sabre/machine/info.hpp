#ifndef _SABRE_MACHINE_INFO_HPP
#define _SABRE_MACHINE_INFO_HPP

/// Sabre Includes
#include "sabre/forward/machine.hpp"

namespace Sabre::Machine {

/// @brief Prepare a native reference typing.
using Callback = Reference (*)(Runtime::Isolate *, const Frame *, const Value::Any &);

/// @brief Machine Code Information.
class Info {
  //  PROPERTIES  //

  /// @brief Maximum local stack count.
  size_t m_locals = 0;

  /// @brief Denotes the size of the machine-code.
  size_t m_bytes = 0;

  /// @brief Underlying jitted callback.
  Callback m_callback = nullptr;

  /// @brief Underlying function information.
  const Function::Info *m_info = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs machine code callee details.
   * @param info              Function information.
   */
  explicit Info(const Function::Info *info = nullptr) : m_info(info) {}

  //  PUBLIC METHODS  //

  /// @brief Minimum/maximum argument counts.
  inline constexpr size_t vargs() const noexcept { return m_info->vargs(); }
  inline constexpr size_t arity() const noexcept { return m_info->arity(); }
  inline constexpr size_t adicity() const noexcept { return m_info->adicity(); }
  inline constexpr size_t argmax() const noexcept { return m_info->argmax(); }
  inline constexpr size_t leaked() const noexcept { return m_info->leaked(); }

  /// @brief Gets the internal bytecode.
  inline constexpr const Image::Slice &bytecode() const noexcept { return m_info->bytecode(); }

  /// @brief Associated variable counts.
  inline constexpr size_t &locals() noexcept { return m_locals; }
  inline constexpr size_t locals() const noexcept { return m_locals; }

  /// @brief Denotes if this is the "main" module function information.
  inline constexpr bool main() const noexcept { return m_info->main(); }

  /// @brief Gets the underlying resource group.
  inline constexpr Resource::Group group() const noexcept { return m_info->group(); }

  /// @brief Gets the underlying bytecode arena.
  inline constexpr const Image::Arena *arena() const noexcept { return m_info->arena(); }

  /// @brief Gets the underlying native code callback.
  inline constexpr Callback &callback() noexcept { return m_callback; }
  inline constexpr Callback callback() const noexcept { return m_callback; }

  /// @brief Gets the underlying native code size.
  inline constexpr size_t &bytes() noexcept { return m_bytes; }
  inline constexpr size_t bytes() const noexcept { return m_bytes; }

  /// @brief Gets the bound resource value.
  inline constexpr $::URI::View resource() const noexcept { return m_info->resource(); }
};

} // namespace Sabre::Machine

#endif
