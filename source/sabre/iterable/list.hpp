#ifndef _SABRE_ITERABLE_LIST_HPP
#define _SABRE_ITERABLE_LIST_HPP

/// Sabre Includes
#include "sabre/iterable/iterator.hpp"

namespace Sabre {

/// @brief List Attributes.
template <> struct Object::Wrapper<Iterable::List> {
  //  PROPERTIES  //

  /// @brief Bound list values.
  std::vector<Value::Any> values = {};

  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an empty/sized list.
   * @param initial               Initial capacity.
   */
  explicit Wrapper(size_t initial = 0) : values(initial) { std::ranges::fill(values, Value::Void()); }

  /**
   * @brief Constructs a list.
   * @param elements              Elements to bind.
   */
  explicit Wrapper(const std::vector<Value::Any> &elements) : values(elements) {}

  /**
   * @brief Constructs a list.
   * @param elements              Elements to bind.
   */
  explicit Wrapper(const std::span<Value::Any> &elements) { $::Ranges::Assign(values, elements); }
  explicit Wrapper(const std::span<const Value::Any> &elements) { $::Ranges::Assign(values, elements); }

  /**
   * @brief Constructs a list.
   * @param isolate               Runtime isolate.
   * @param iterator              Generator value.
   */
  explicit Wrapper(Runtime::Isolate *isolate, const Iterable::Iterator &iterator) {
    while (!iterator.next(isolate)) values.emplace_back(iterator.value());
  }
};

/// @brief List Interface.
struct Iterable::List : public Object::Mixin<Iterable::List> {
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Mixin::Mixin;

  //  PUBLIC METHODS  //

  /// @brief Denotes if the list is empty.
  inline constexpr bool empty() const { return m_wrapper()->values.empty(); }

  /// @brief Gets the size of the list.
  inline constexpr size_t size() const { return m_wrapper()->values.size(); }

  /// @brief Gets the available values from the list.
  inline constexpr Value::Any *data() const { return m_wrapper()->values.data(); }
  inline constexpr std::span<Value::Any> span() const { return {data(), size()}; }
  inline constexpr std::vector<Value::Any> &values() const { return m_wrapper()->values; }

  /// @brief Handles clearing the list of values.
  inline constexpr const Iterable::List &clear() const { return values().clear(), *this; }

  /// @brief Gets the front-most value.
  inline constexpr Value::Any front() const { return get(0); }

  /// @brief Gets the back-most value.
  inline constexpr Value::Any back() const { return get(size() - 1); }

  /**
   * @brief Allow slicing spans.
   * @param offset                Offset to slice.
   * @param count                 Total slice count.
   */
  inline constexpr std::span<Value::Any> slice(size_t offset, size_t count = std::dynamic_extent) const {
    return span().subspan(offset, count);
  }

  /**
   * @brief Handles getting a list-value.
   * @param index                 Index of value.
   */
  inline constexpr Value::Any get(size_t index) const noexcept {
    $_UNUSED $_AUTO = m_guard(index);
    return m_wrapper()->values[index];
  }

  /**
   * @brief Handles setting a list-value.
   * @param index                 Index of value.
   * @param value                 Value to set.
   */
  inline constexpr Value::Any set(size_t index, const Value::Any &value) const noexcept {
    $_UNUSED $_AUTO = m_guard(index);
    return m_wrapper()->values[index] = value;
  }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Prepares a suitably guarded handler.
   * @param index                 Index to validate.
   */
  inline constexpr Object::Guard m_guard($_UNUSED size_t index) const noexcept {
    return $_ASSERT(index < size(), "Index {0} exceeds list-size {1}", index, size()), Mixin::m_guard();
  }

  /**
   * @brief Handlers yielding lists safely.
   * @param self                          List instance.
   * @param yield                         Yield callback.
   */
  static void m_yield(const List &self, Globals::Each &yield);

  /**
   * @brief Handles printing values.
   * @param os                    Output stream.
   * @param self                  List instance.
   */
  static inline void m_print(std::ostream &os, const List &self) {
    os << $::Dye::cyan("<{0}: size({1})>", self.brand(), self.size());
  }
};

} // namespace Sabre

#endif
