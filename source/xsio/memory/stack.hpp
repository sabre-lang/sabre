#ifndef _XSIO_MEMORY_STACK_HPP
#define _XSIO_MEMORY_STACK_HPP

/// XSIO Includes
#include "xsio/forward/virtual.hpp"
#include "xsio/memory/buffer.hpp"

namespace XSIO::Memory {

/// @brief Thread Stack Declaration.
class Stack {
  //  PROPERTIES  //

  /// @brief Underlying buffer instance.
  Buffer m_buffer;

  /// @brief The available underflow limit.
  size_t m_underflow;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a thread-stack.
   * @param size                  Size of stack.
   * @param underflow             Underflow limit.
   */
  constexpr Stack(size_t size = 0, size_t underflow = $::Memory::Size::KB) : m_buffer(size), m_underflow(underflow) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the size of the stack.
  inline constexpr size_t size() const noexcept { return m_buffer.size(); }

  /// @brief Gets the underlying stack buffer.
  inline constexpr const Buffer &buffer() const noexcept { return m_buffer; }

  /// @brief Gets the low-offset address.
  inline constexpr void *low() const noexcept { return m_buffer.data(); }

  /// @brief Gets the high-offset address.
  inline constexpr void *high() const noexcept { return reinterpret_cast<void *>(m_buffer.address() + size()); }

  /// @brief Gets the current usage of the stack.
  inline constexpr size_t usage() const noexcept { return size() - remaining(); }

  /// @brief Gets the remaining stack-size of a thread.
  inline constexpr uintptr_t remaining() const noexcept { return m_frame() - m_buffer.address(); }

  /// @brief Denotes if the stack has exceeded its underflow range.
  inline constexpr bool overflowed() const noexcept { return remaining() < m_underflow; }

private:
  //  PRIVATE METHODS  //

  /// @brief Gets the frame address available.
  static inline constexpr uintptr_t m_frame() noexcept {
    return reinterpret_cast<uintptr_t>(__builtin_frame_address(0));
  }
};

} // namespace XSIO::Memory

#endif
