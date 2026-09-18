#ifndef _SABRE_ITERABLE_ITERATOR_HPP
#define _SABRE_ITERABLE_ITERATOR_HPP

/// Sabre Includes
#include "sabre/iterable/callback.hpp"

namespace Sabre {

/// @brief Iterator Attributes.
template <> struct Object::Wrapper<Iterable::Iterator> {
  //  TYPEDEFS  //

  /// @brief Allow the iterator internal access.
  friend struct Iterable::Iterator;

  //  PROPERTIES  //

  /// @brief Internal index value.
  size_t index = 0;

  /// @brief Current iterator value.
  Value::Any value = {};

  /// @brief Iterable state value (usually the iterator).
  Value::Any state = {};

  /// @brief Handles getting the subsequent value.
  Iterable::Callback<Value::Any> next = nullptr;

  //  CONSTRUCTORS  //

  /// @brief Constructs an empty iterator.
  explicit Wrapper() : value(Value::Sentinel()) {}

  /**
   * @brief Constructs a typed iterator.
   * @param iterable              Iterable state.
   * @param callback              Typed callback.
   */
  template <std::derived_from<Value::Any> T>
  explicit Wrapper(const T &iterable, Iterable::Callback<T> &&callback) : state(iterable) {
    next = [cb = std::move(callback)](Runtime::Isolate *isolate, const Value::Any &self, size_t index) -> Value::Any {
      return cb(isolate, self.as<T>(), index); // and convert into our suitable callback now
    };
  }

  /**
   * @brief Constructs an untyped iterator.
   * @param iterable              Iterable state.
   * @param callback              Untyped callback.
   */
  template <>
  explicit Wrapper<Value::Any>(const Value::Any &iterable, Iterable::Callback<Value::Any> &&callback) :
      state(iterable), next(std::move(callback)) {
    if ($_UNLIKELY(callback == nullptr)) value = Value::Sentinel();
  }

private:
  //  PRIVATE METHODS  //

  /// @brief Denotes if the result is alright or not.
  inline constexpr bool m_okay() const noexcept { return value.pointer().okay(); }

  /// @brief Denotes if the iterator is done.
  inline constexpr bool m_done() const noexcept { return value.pointer().sentinel(); }

  /**
   * @brief Handles advancing the iterable.
   * @param isolate               Thread isolate.
   */
  inline constexpr bool m_advance(Runtime::Isolate *isolate) {
    if ($_UNLIKELY(next == nullptr) || m_done()) return true;
    return value = next(isolate, state, index++), m_done();
  }
};

/// @brief Iterator Interface.
struct Iterable::Iterator : public Object::Mixin<Iterable::Iterator> {
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Mixin::Mixin;

  //  PUBLIC METHODS  //

  /// @brief Denotes if the iterator has finished.
  inline constexpr bool done() const noexcept { return m_wrapper()->m_done(); }

  /// @brief Denotes if the current value is safe.
  inline constexpr bool okay() const noexcept { return m_wrapper()->m_okay(); }

  /// @brief Gets the current iterator index.
  inline constexpr size_t index() const noexcept { return m_wrapper()->index; }

  /// @brief Gets the current iterator value.
  inline constexpr Value::Any value() const noexcept { return m_wrapper()->value; }

  /**
   * @brief Handles advancing the iterator.
   * @param isolate               Thread isolate.
   */
  inline constexpr bool next(Runtime::Isolate *isolate) const noexcept { return m_wrapper()->m_advance(isolate); }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handlers yielding iterators safely.
   * @param self                          Iterator instance.
   * @param yield                         Yield callback.
   */
  static void m_yield(const Iterator &self, Globals::Each &yield);
};

} // namespace Sabre

#endif
