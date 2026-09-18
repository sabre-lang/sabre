#ifndef _SABRE_BUILTINS_ENUM_HPP
#define _SABRE_BUILTINS_ENUM_HPP
/// Sabre Includes
#include "sabre/builtins/wrapper.hpp"

namespace Sabre::Builtins {

/// @brief Tagged Enumeration Builtin Traits.
template <> struct Wrapper<Object::Enum> : public Blueprint<Object::Enum, "Enum"> {
  //  PUBLIC METHODS  //

  /// @brief Gets the baseline list type-class.
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
   * @brief Handles looking up enumeration fields.
   * @param self                      Enum object.
   * @param symbol                    Field symbol.
   */
  static Member::View m_attribute(const Object::Enum &self, const Value::Symbol &symbol);
};

} // namespace Sabre::Builtins

#endif
