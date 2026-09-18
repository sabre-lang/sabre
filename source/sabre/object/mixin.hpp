#ifndef _SABRE_OBJECT_MIXIN_HPP
#define _SABRE_OBJECT_MIXIN_HPP

/// Sabre Includes
#include "sabre/object/common.hpp"
#include "sabre/object/guard.hpp"
#include "sabre/value/void.hpp"

namespace Sabre::Object {

/// @brief Allocatable Object Attributes Wrapper.
template <class T> struct Wrapper;

/// @brief Object Abstraction CRPT.
template <class T> struct Mixin : public Value::Any {
  //  TYPEDEFS  //

  /// @brief Ensure that the attributes allocation will fit.
  static_assert(sizeof(Wrapper<T>) <= UINT8_MAX);

  //  CONSTRUCTORS  //

  /**
   * @brief Consturcts a tagged object.
   * @param pointer                   Optional pointer.
   */
  constexpr Mixin() : Value::Any(Object::Any()) {}
  constexpr Mixin(Pointer::Underlying pointer) : Value::Any(pointer) {
    $_ASSERT(m_is(m_pointer), $::RTTI::Name<T>(), m_pointer);
  }

protected:
  //  PRIVATE METHODS  //

  /// @brief Constructs a suitable value guard.
  inline constexpr Guard m_guard() const noexcept { return Guard(*this); }

  /// @brief Gets the underlying object header.
  inline constexpr Header *m_header() const noexcept { return m_as<Object::Any>()->header(); }

  /// @brief Gets the object address.
  inline constexpr Pointer::Underlying m_address() const noexcept { return m_as<Object::Any>()->address(); }

  /// @brief Gets the underlying attributes wrapper.
  inline constexpr Wrapper<T> *m_wrapper() const noexcept { return std::bit_cast<Wrapper<T> *>(m_address()); }

  /**
   * @brief Validates incoming pointers.
   * @param pointer                   Pointer to validate.
   */
  static inline constexpr bool m_is(const Pointer::Tagged &pointer) {
    return Object::Any::m_is(pointer) && Object::Any(pointer).header()->is<T>();
  }

  /**
   * @brief Forces calls for truthiness to query base.
   * @param self                      Value to resolve.
   */
  static inline constexpr bool m_truthiness(const Any &self) { return self.truthiness(); }

  /**
   * @brief Handles printing generic values.
   * @param os                        Output stream.
   * @param self                      Object value.
   */
  static inline void m_print(std::ostream &os, const T &self) { os << *self.template m_as<Object::Any>(); }
};

} // namespace Sabre::Object

#endif
