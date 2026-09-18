#ifndef _SABRE_BYTECODE_TRACE_HPP
#define _SABRE_BYTECODE_TRACE_HPP

/// Sabre Includes
#include "sabre/forward/bytecode.hpp"

namespace Sabre::Bytecode {

/// @brief Bytecode Source Trace.
class Trace {
  //  PROPERTIES  //

  /// @brief Bound trace position.
  XLSP::Position m_position = {};

  /// @brief Compilation stack.
  std::stack<XLSP::Position> *m_stack = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs an empty trace.
  explicit constexpr Trace() = default;

  /**
   * @brief Constructs a bytecode-trace.
   * @param stack             Stack to append to.
   * @param position          Current position.
   */
  explicit constexpr Trace(std::stack<XLSP::Position> *stack, const XLSP::Position &position = {}) :
      m_position(position), m_stack(stack) {
    if (valid()) m_stack->push(m_position);
  }

  /// @brief Handles removing the trace instance.
  constexpr ~Trace() {
    if (valid()) $_ASSERT(m_stack->top() == m_position), m_stack->pop();
  }

  //  PUBLIC METHODS  //

  /// @brief Gets the current trace position.
  inline constexpr const XLSP::Position &position() const noexcept { return m_position; }

  /// @brief Checks if this position is valid.
  inline constexpr bool valid() const noexcept { return m_stack && m_position != XLSP::Position(); }
};

} // namespace Sabre::Bytecode

#endif
