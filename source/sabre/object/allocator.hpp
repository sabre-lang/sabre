#ifndef _SABRE_OBJECT_ALLOCATOR_HPP
#define _SABRE_OBJECT_ALLOCATOR_HPP

/// Sabre Includes
#include "sabre/heap/options.hpp"
#include "sabre/object/mixin.hpp"

namespace Sabre::Object {

/// @brief Object Allocator.
struct Allocator {
  //  PUBLIC METHODS  //

  /**
   * @brief Handles constructing values.
   * @param isolate               Runtime isolate.
   * @param args                  Constructor arguments.
   */
  template <std::derived_from<Value::Any> T, class... As>
  static inline constexpr T create(Runtime::Isolate *isolate, As &&...args) {
    if constexpr (!m_validate<T>()) return m_inlined<T>(isolate, std::forward<As>(args)...);
    else return m_create<T>(isolate, std::forward<As>(args)...); // should be built here
  }

  /**
   * @brief Handles moving values.
   * @param address               Address to move into.
   * @param value                 Value to be moved.
   */
  template <std::derived_from<Value::Any> T>
  static inline constexpr void move(Heap::Address address, const Value::Any &value) {
    if constexpr (m_validate<T>()) m_move<T>(address, value.as<Object::Any>());
  }

  /**
   * @brief Handles destructing values.
   * @param value                 Value to destruct.
   */
  template <std::derived_from<Value::Any> T> static inline constexpr void destroy(const Value::Any &value) {
    if constexpr (m_validate<T>()) m_destruct<T>(value.as<Object::Any>());
  }

private:
  //  PRIVATE METHODS  //

  /// @brief Validates incoming value
  template <std::derived_from<Value::Any> T> static inline constexpr bool m_validate() {
    return std::derived_from<T, Mixin<T>>;
  }

  /**
   * @brief Handles creating an object.
   * @param isolate               Runtime isolate.
   * @param args                  Constructor arguments.
   */
  template <std::derived_from<Value::Any> T, class... As>
  static inline constexpr T m_create(Runtime::Isolate *isolate, As &&...args) {
    using Wrapper = typename Object::Wrapper<T>; // resolve details
    using Underlying = std::conditional_t<std::same_as<T, Any>, Instance, T>;

    // get the base details about the wrapper now
    static constexpr auto s_size = sizeof(Wrapper);
    static constexpr auto s_shape = Shape::Lookup<Underlying>();
    $_UNUSED static constexpr auto s_name = $::RTTI::Name<Underlying>();

    // ensure that the desired object is actually valid to be constructed
    $_ASSERT(!!s_size, "Object '{0}' does not have any attributes", s_name);
    $_ASSERT(s_size <= UINT8_MAX, "Object '{0}' attributes exceed maximum size", s_name);

    // attempt allocating the underlying memory now
    auto address = m_new(isolate, s_size, s_shape);

    // construct the actual details now
    m_construct<Wrapper>(isolate, address, std::forward<As>(args)...);

    // and return the resulting encoded object instance
    return T(Any::m_encode(address));
  }

  /**
   * @brief Constructs any valid value.
   * @param isolate               Runtime isolate.
   * @param args                  Constructor arguments.
   */
  template <std::derived_from<Value::Any> T, class... As>
  static inline constexpr T m_inlined(Runtime::Isolate *isolate, As &&...args) {
    static constexpr auto without = std::constructible_from<T, As...>;
    if constexpr (without) return T(std::forward<As>(args)...);
    else return T(isolate, std::forward<As>(args)...);
  }

  /**
   * @brief Handles constructing an object.
   * @param isolate               Runtime isolate.
   * @param address               Address to allocate.
   * @param args                  Constructor arguments.
   */
  template <class T, class... As>
  static inline constexpr void m_construct(Runtime::Isolate *isolate, Heap::Address address, As &&...args) {
    auto *attributes = std::bit_cast<T *>(address); // get the attributes
    static constexpr auto s_without = std::constructible_from<T, As...>;
    if constexpr (s_without) new (attributes) T(std::forward<As>(args)...);
    else new (attributes) T(isolate, std::forward<As>(args)...);
  }

  /**
   * @brief Handles moving values.
   * @param address               Address to move into.
   * @param value                 Value to be moved.
   */
  template <std::derived_from<Value::Any> T>
  static inline constexpr void m_move(Heap::Address address, Object::Any object) {
    using Wrapper = typename Object::Wrapper<T>;
    auto *other = std::bit_cast<Wrapper *>(object.address());
    new (std::bit_cast<Wrapper *>(address)) Wrapper(std::move(*other));
  }

  /**
   * @brief Handles destructing an object.
   * @param object                Value to destruct.
   */
  template <std::derived_from<Value::Any> T> static inline constexpr void m_destruct(Object::Any object) {
    using Wrapper = typename Object::Wrapper<T>;
    std::bit_cast<Wrapper *>(object.address())->~Wrapper();
  }

  /**
   * @brief Handles allocating an object.
   * @param isolate               Runtime isolate.
   * @param size                  Size to allocate.
   * @param shape                 Object shape value.
   */
  static Heap::Address m_new(Runtime::Isolate *isolate, size_t size, Shape::Underlying shape);
};

} // namespace Sabre::Object

#endif
