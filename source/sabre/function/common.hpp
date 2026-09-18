#ifndef _SABRE_FUNCTION_ANY_HPP
#define _SABRE_FUNCTION_ANY_HPP

/// Sabre Includes
#include "sabre/function/closure.hpp"
#include "sabre/function/jitted.hpp"
#include "sabre/function/native.hpp"

namespace Sabre::Function {

/// @brief Any Function Pointer.
struct Any : public Value::Any {
  //  CONSTRUCTORS  //

  /// @brief Constructs an empty object.
  constexpr Any() : Value::Any(Object::Any()) {}

  /**
   * @brief Constructs a dynamic function.
   * @param pointer           Pointer to function.
   */
  constexpr Any(Pointer::Underlying pointer) : Value::Any(pointer) { $_ASSERT(is<Any>()); }

  /**
   * @brief Constructs a dynamic function.
   * @param underlying        Underlying function.
   */
  constexpr Any(const Native &underlying) : Value::Any(underlying) {}
  constexpr Any(const Jitted &underlying) : Value::Any(underlying) {}
  constexpr Any(const Closure &underlying) : Value::Any(underlying) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the minimum allowed arguments.
  inline constexpr size_t arity() const noexcept {
    return m_apply<size_t>(0, [](auto value) { return value->arity(); });
  }

  /// @brief Gets the maximum allowed arguments.
  inline constexpr size_t adicity() const noexcept {
    return m_apply<size_t>(SIZE_MAX, [](auto value) { return value->adicity(); });
  }

  /// @brief Gets the associated function resource.
  inline constexpr $::URI::View resource() const noexcept {
    return m_apply<$::URI::View>({}, [](auto value) { return value->resource(); });
  }

  /// @brief Gets the associated receiver value.
  inline constexpr Value::Any receiver() const noexcept {
    return m_apply<Value::Any>(Value::Void(), [](auto value) { return value->receiver(); });
  }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles applying a string callback.
   * @param callback                      Callback to run.
   */
  template <class R, class F> inline constexpr R m_apply(const R &preset, F callback) const noexcept {
    // handle based on the incoming shape
    switch (m_as<Object::Any>()->header()->shape()) {
    case Shape::Lookup<Native>(): return callback(m_as<Native>());
    case Shape::Lookup<Jitted>(): return callback(m_as<Jitted>());
    case Shape::Lookup<Closure>(): return callback(m_as<Closure>());

    // default to the base instance
    default: return preset;
    }
  }

  /**
   * @brief Checks for valid functions.
   * @param traits            Pointer traits.
   */
  static inline constexpr bool m_is(const Pointer::Tagged &pointer) {
    if (pointer.kind() < Pointer::Kind::OBJ) return false;
    return Object::Any(pointer).header()->is<Native, Closure, Jitted>();
  }

  /**
   * @brief Handles printing values.
   * @param os                Output stream.
   * @param self              Dynamic function.
   */
  static inline void m_print(std::ostream &os, const Any &self) { os << *self.m_as<Object::Any>(); }
};

} // namespace Sabre::Function

#endif
