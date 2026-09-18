#ifndef _SABRE_FUNCTION_ARGS_HPP
#define _SABRE_FUNCTION_ARGS_HPP

/// Sabre Includes
#include "sabre/number/tagged.hpp"
#include "sabre/value/void.hpp"

/*
 *
 * Function arguments support the following layout.
 *
 * |    LABEL    |  OFFSET  |         DESCRIPTION          |
 * | ----------- | -------- | ---------------------------- |
 * |  ARGS_SIZE  |    0     | Size of the base arguments.  |
 * |  ARGS_SELF  |    1     | The arguments self value.    |
 * |  ARGS_DATA  |    2     | The available arguments.     |
 *
 */

namespace Sabre::Function::Offset {

//  PROPERTIES  //

static inline constexpr size_t ARGS_SIZE = 0;             // Argument size.
static inline constexpr size_t ARGS_SELF = ARGS_SIZE + 1; // Receiver value.
static inline constexpr size_t ARGS_DATA = ARGS_SELF + 1; // Arguments data.

static inline constexpr int32_t STK_OPTR = -4; // Offset pointer.
static inline constexpr int32_t STK_ENVP = -3; // Environment contextt.
static inline constexpr int32_t STK_SIZE = -2; // Arguments size.
static inline constexpr int32_t STK_SELF = -1; // Receiver value.
static inline constexpr int32_t STK_ACCU = +0; // Frame accumulator.

} // namespace Sabre::Function::Offset

namespace Sabre::Function {

/// @brief Function Callsite Arguments.
class Args {
  //  PROPERTIES  //

  /// @brief Span of available values.
  Value::Any *m_span = nullptr;

  // prepare the incoming data offset and size
  static constexpr auto s_offset = static_cast<size_t>(Offset::ARGS_DATA);

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a sized set of arguments.
   * @param capacity           Capacity count.
   */
  constexpr Args(size_t count) { m_reserve(count); }

  /**
   * @brief Constructs callsite arguments.
   * @param span              Local arguments.
   */
  constexpr Args(const std::span<Value::Any> &span = {}) { m_reserve(span.size(), span.data()); }

  /**
   * @brief Constructs callsite arguments.
   * @param self              Self argument.
   * @param span              Local arguments.
   */
  constexpr Args(const Value::Any &self, const std::span<Value::Any> &span = {}) {
    m_reserve(span.size(), span.data(), self);
  }

  /// @brief Copy-constructor for arguments.
  constexpr Args(const Args &other) { m_copy(other.size(), other.m_span); }

  /// @brief Move-constructor for arguments.
  constexpr Args(Args &&other) : m_span(std::move(other.m_span)) { other.m_span = nullptr; }

  /// @brief Handles destroying the constructed memory.
  ~Args() { m_destroy(); }

  //  OPERATOR METHODS  //

  /// @brief Handles getting an argument value.
  inline constexpr Value::Any operator[](size_t index) const noexcept {
    return index < size() ? data()[index] : Value::Void();
  }

  /// @brief Handles copy-assignment.
  inline constexpr Args &operator=(const Args &other) {
    // ignore when self-assignment occurs
    if (this == &other) return *this;

    // let's destroy and reserve this item
    m_destroy(), m_copy(other.size(), other.m_span);

    // return the resulting instance now
    return *this;
  }

  /// @brief Handles move-assignment.
  inline constexpr Args &operator=(Args &&other) {
    // ignore when self-assignment occurs
    if (this == &other) return *this;

    // copy the incoming details
    m_span = std::move(other.m_span);

    // clear the original now
    other.m_span = nullptr;

    // and return the original reference
    return *this;
  }

  //  PUBLIC METHODS  //

  /// @brief Denotes if the arguments are empty.
  inline constexpr bool empty() const noexcept { return size() == 0; }

  /// @brief Gets the size of the available arguments.
  inline constexpr size_t size() const noexcept { return m_get(Offset::ARGS_SIZE)->pointer(); }

  /// @brief Gets the bound self value.
  inline constexpr Value::Any &self() noexcept { return m_self(); }
  inline constexpr const Value::Any &self() const noexcept { return m_self(); }

  /// @brief Gets the underlying data.
  inline constexpr Value::Any *data() const noexcept { return m_get(Offset::ARGS_DATA); }

  /// @brief Gets a span of the arguments.
  inline constexpr std::span<Value::Any> span() const noexcept { return {data(), size()}; }
  inline constexpr std::span<Value::Any> span(size_t offset) const noexcept {
    return {data() + offset, size() - offset};
  }

  /// @brief Gets the self value cast into another type.
  template <std::derived_from<Value::Any> T> inline constexpr T self() const noexcept { return m_self().as<T>(); }

  /**
   * @brief Allows casting a value.
   * @param index                 Index of value.
   */
  template <std::derived_from<Value::Any> T = Value::Any> inline constexpr T at(size_t index) const noexcept {
    if constexpr (std::same_as<T, Value::Any>) return (*this)[index];
    else return (*this)[index].as<T>(); // force a value cast to occur
  }

  /**
   * @brief Allows casting a value.
   * @param index                 Index of value.
   * @param alternative           Defaulted value.
   */
  template <std::derived_from<Value::Any> T = Value::Any, std::derived_from<Value::Any> A = T>
  inline constexpr T at(size_t index, A alternative) const noexcept {
    return index < size() ? at<T>(index) : alternative;
  }

  /**
   * @brief Handles slicing arguments.
   * @param offset                Offset to slice.
   * @param count                 Count to be used.
   */
  inline constexpr std::span<Value::Any> slice(size_t offset, size_t count = std::dynamic_extent) noexcept {
    return span().subspan(offset, count);
  }

  /**
   * @brief Handles slicing arguments.
   * @param offset                Offset to slice.
   * @param count                 Count to be used.
   */
  inline constexpr std::span<Value::Any> slice(size_t offset, size_t count = std::dynamic_extent) const noexcept {
    return span().subspan(offset, count);
  }

private:
  //  PRIVATE METHODS  //

  /// @brief Gets the underlying self instance.
  inline constexpr Value::Any &m_self() const noexcept { return *m_get(Offset::ARGS_SELF); }

  /// @brief Handles destroying a set of arguments.
  inline constexpr void m_destroy() {
    if (m_span) delete[] m_span;
    m_span = nullptr; // clear
  }

  /**
   * @brief Handles getting a value at an offset.
   * @param offset                Offset to get.
   */
  inline constexpr Value::Any *m_get(size_t offset) const noexcept {
    return $_ASSERT(m_span != nullptr), m_span + offset;
  }

  /**
   * @brief Handles copying a set of arguments
   * @param count                 Count to copy.
   * @param data                  Data to copy.
   */
  inline void m_copy(size_t count, Value::Any *data = nullptr) {
    // ensure the incoming span is empty currently
    if ($_UNLIKELY(m_span != nullptr)) return;

    // construct the incoming span now
    m_span = new Value::Any[count + s_offset];

    // copy across the incoming data now if given
    std::memcpy(m_span, data, sizeof(Value::Any) * (count + s_offset));
  }

  /**
   * @brief Gets the reservation cost ([size, self] + capacity).
   * @param count                 Capacity to build.
   * @param data                  Initial values.
   * @param self                  Self value to bind.
   */
  inline void m_reserve(size_t count, Value::Any *data = nullptr, const Value::Any &self = {}) noexcept {
    // ensure the incoming span is empty currently
    if ($_UNLIKELY(m_span != nullptr)) return;

    // construct the incoming span now
    m_span = new Value::Any[count + s_offset];

    // copy across the incoming data now if given
    if (data) std::memcpy(m_span + s_offset, data, sizeof(Value::Any) * count);

    // always forcibly update the incoming size
    m_self() = self, *m_get(Offset::ARGS_SIZE) = Value::Any(count);
  }
};

} // namespace Sabre::Function

#endif
