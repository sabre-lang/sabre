#ifndef _SABRE_BUILTINS_OBJECT_HPP
#define _SABRE_BUILTINS_OBJECT_HPP

/// Sabre Includes
#include "sabre/builtins/wrapper.hpp"

namespace Sabre::Builtins {

/// @brief Instance Builtin Traits.
template <> struct Wrapper<Object::Instance> : public Blueprint<Object::Instance, "Object"> {
  //  PUBLIC METHODS  //

  /// @brief Gets the baseline object type-class.
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
   * @brief Handles looking up object fields.
   * @param self                      Object instance.
   * @param symbol                    Field symbol.
   */
  static Member::View m_attribute(const Object::Instance &self, const Value::Symbol &symbol);
};

/// @brief Object Builtin Traits.
template <> struct Wrapper<Object::Any> : public Wrapper<Object::Instance> {};

} // namespace Sabre::Builtins

#endif
