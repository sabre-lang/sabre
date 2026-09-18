#ifndef _SABRE_VALUE_ANY_HPP
#define _SABRE_VALUE_ANY_HPP

/// Sabre Includes
#include "sabre/forward/handle.hpp"
#include "sabre/member/descriptor.hpp"
#include "sabre/operator/attribute.hpp"
#include "sabre/value/inspect.hpp"

namespace Sabre::Value {

/// @brief Base Runtime Value.
class Any {
  //  TYPEDEFS  //

  /// @brief Allow object guards internal access.
  friend class Object::Guard;

  /// @brief Allow locals internal access.
  template <std::derived_from<Any> T> friend class Handle::Local;

protected:
  //  PROPERTIES  //

  /// @brief The underlying tagged pointer.
  Pointer::Tagged m_pointer = Pointer::Tagged();

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a defaulted value.
  constexpr Any() = default;

  /**
   * @brief Constructs a value.
   * @param tagged          Tagger pointer.
   */
  constexpr Any(const Pointer::Tagged &tagged) : m_pointer(tagged) {}

  //  OPERATOR METHODS  //

  /// @brief Allows checking for direct equality of values.
  inline constexpr bool operator==(const Any &other) const noexcept { return m_equals(other); }
  inline constexpr bool operator!=(const Any &other) const noexcept { return !m_equals(other); }

  //  PUBLIC METHODS  //

  /// @brief Gets the truthiness of the value.
  inline constexpr bool truthiness() const noexcept { return m_truthiness(); }

  /// @brief Gets the name of the value type.
  inline constexpr $::String::View brand() const noexcept { return m_brand(); }

  /// @brief Gets the underlying tagged pointer value.
  inline constexpr const Pointer::Tagged &pointer() const noexcept { return m_pointer; }

  /// @brief Checks if the value is a particular typing.
  template <std::derived_from<Any> T> inline constexpr bool is() const noexcept { return Inspect<T>::is(m_pointer); }

  /// @brief Casts values to a desired one (safe in debug-mode).
  template <std::derived_from<Any> T> inline constexpr T as() const noexcept {
    $_UNUSED static constexpr auto s_to = $::RTTI::Name<T>(); // prepare the cast name
    return $_ASSERT(is<T>(), "Failed cast from '{0}' to '{1}'", brand(), s_to), *m_as<T>();
  }

  /**
   * @brief Handles resolving attributes.
   * @param field                   Field to resolve.
   */
  inline constexpr Member::View attribute(const Symbol &symbol) const noexcept { return m_attribute(symbol); }
  inline constexpr Member::View attribute(Operator::Kind kind) const noexcept { return m_attribute(kind); }

protected:
  //  PRIVATE METHODS  //

  /// @brief Allows casting values to derived types.
  template <std::derived_from<Any> T> $_INLINE_PERF constexpr const T *m_as() const noexcept {
    if constexpr (std::same_as<T, Value::Any>) return this;
    else return static_cast<const T *>(this); // cast derived
  }

  /// @brief Gets the truthiness of the value.
  bool m_truthiness() const noexcept;

  /// @brief Gets the type-name of the value.
  $::String::View m_brand() const noexcept;

  /**
   * @brief Handles resolving attributes.
   * @param field                   Field to resolve.
   */
  Member::View m_attribute(const Symbol &symbol) const noexcept;
  Member::View m_attribute(Operator::Kind kind) const noexcept;

  /**
   * @brief Checks if two values are equal.
   * @param other                   Other value to check.
   */
  bool m_equals(const Value::Any &other) const noexcept;

  /// @brief Fallback validation check (all values succeed base-case).
  static bool m_is(const Pointer::Tagged &);

  /**
   * @brief Handles printing values.
   * @param os              Output stream.
   * @param self            Value to print.
   */
  static void m_print(std::ostream &os, const Any &self);
};

//  PUBLIC METHODS  //

/// @brief Allows unsafely casting to other values.
template <std::derived_from<Any> T = Value::Any, class V> inline constexpr T Cast(const V &value) noexcept {
  if constexpr (std::same_as<T, V>) return value; // same type
  else if constexpr (std::derived_from<V, Any>) return value.template as<T>();
  else return *reinterpret_cast<const T *>(&value); // reinterpret via pointer
}

} // namespace Sabre::Value

#endif
