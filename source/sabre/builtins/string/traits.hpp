#ifndef _SABRE_BUILTINS_STRING_HPP
#define _SABRE_BUILTINS_STRING_HPP

/// Sabre Includes
#include "sabre/builtins/wrapper.hpp"

namespace Sabre::Builtins {

/// @brief Dynamic String Builtin Traits.
template <> struct Wrapper<String::Any> : public Blueprint<String::Any, "String", Adapter::OPERATORS> {
  //  PUBLIC METHODS  //

  /// @brief Gets the baseline string type-class.
  static $::Shared::Pointer<Type::Prototype> typeclass();

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles defining global type definitions.
   * @param globals                     Global type-world.
   */
  static void m_typedefs(Type::World *globals);

  /**
   * @brief Handles instantiating globals.
   * @param isolate                   Runtime isolate.
   * @param prototype                 Prototype instance.
   */
  static Value::Any m_globals(Isolate *isolate, const Object::Class &prototype);

  /**
   * @brief Handles looking up value fields.
   * @param self                      Value instance.
   * @param symbol                    Field symbol.
   */
  static Member::View m_attribute(const String::Any &self, const Value::Symbol &symbol);

  /**
   * @brief Handles looking up operator methods.
   * @param self                      Value instance.
   * @param kind                      Operator kind.
   */
  static Member::View m_operator(const String::Any &self, Operator::Kind kind);
};

/// @brief Small String Builtin Traits.
template <> struct Wrapper<String::Small> : public Wrapper<String::Any> {};

/// @brief Large String Builtin Traits.
template <> struct Wrapper<String::Large> : public Wrapper<String::Any> {};

} // namespace Sabre::Builtins

#endif
