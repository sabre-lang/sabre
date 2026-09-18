#ifndef _SABRE_RUNTIME_STACK_HPP
#define _SABRE_RUNTIME_STACK_HPP

/// Sabre Includes
#include "sabre/forward/runtime.hpp"
#include "sabre/heap/buffer.hpp"
#include "sabre/object/allocator.hpp"
#include "sabre/value/common.hpp"

namespace Sabre::Runtime {

/// @brief Temporary Stack Container.
class Stack {
  //  PROPERTIES  //

  /// @brief The capacity bound.
  size_t m_capacity = 0;

  /// @brief Associated isolate.
  Isolate *m_isolate = nullptr;

  /// @brief The leading stack pointer.
  Value::Any *m_data = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs an empty stack.
  explicit constexpr Stack() = default;

  /**
   * @brief Constructs a runtime stack.
   * @param isolate               Runtime isolate.
   * @param capacity              Total capacity.
   */
  explicit constexpr Stack(Isolate *isolate, size_t capacity = 0) :
      m_capacity(m_align(capacity)), m_isolate(isolate), m_data(m_allocate()) {}

  /// @brief Handles destructing the incoming stack.
  constexpr ~Stack() { m_release(); }

  //  OPERATOR METHODS  //

  /// @brief Allows accessing and modifying stack values.
  inline constexpr Value::Any &operator[](size_t index) noexcept { return m_data[index]; }
  inline constexpr Value::Any operator[](size_t index) const noexcept { return m_data[index]; }

  //  PUBLIC METHODS  //

  inline constexpr size_t size() const noexcept { return m_capacity; }
  inline constexpr Value::Any *data() const noexcept { return m_data; }
  inline constexpr std::span<Value::Any> span() const noexcept { return {data(), size()}; }

private:
  //  PRIVATE METHODS  //

  /// @brief Handles allocating the stack.
  Value::Any *m_allocate();

  /// @brief Handles releasing the stack.
  void m_release();

  /**
   * @brief Resolves a suitable alignment.
   * @param size                      Size to align.
   */
  static inline constexpr size_t m_align(size_t size) {
    return Heap::Align<Pointer::Alignment::OBJECT / sizeof(Value::Any)>(size);
  }
};

/// @brief Runtime Memory Allocator.
class Allocator : private $::Ensure::Static {
  //  TYPEDEFS  //

  /// @brief Allow the stack internal access.
  friend class Stack;

public:
  //  PUBLIC METHODS  //

  /**
   * @brief Constructs a temporary stack.
   * @param capacity              Size of the stack.
   */
  static inline constexpr Stack stack(Isolate *isolate, size_t capacity) { return Stack(isolate, capacity); }

  /**
   * @brief Handles constructing objects.
   * @param args                  Attribute arguments.
   */
  template <std::derived_from<Value::Any> T, class... As>
  static inline constexpr T create(Isolate *isolate, As &&...args) {
    return Object::Allocator::create<T>(isolate, std::forward<As>(args)...);
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles reserving data.
   * @param isolate               Runtime isolate.
   * @param size                  Size of incoming data.
   */
  static Value::Any *m_reserve(Isolate *isolate, size_t size);

  /**
   * @brief Handles releasing data.
   * @param isolate               Runtime isolate.
   * @param data                  Data to release.
   * @param size                  Size of data.
   */
  static void m_release(Isolate *isolate, Value::Any *data, size_t size);
};

} // namespace Sabre::Runtime

#endif
