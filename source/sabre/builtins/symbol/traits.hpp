#ifndef _SABRE_BUILTINS_SYMBOL_HPP
#define _SABRE_BUILTINS_SYMBOL_HPP

/// Sabre Includes
#include "sabre/builtins/wrapper.hpp"

namespace Sabre::Builtins {

/// @brief Tagged Symbol Builtin Traits.
template <> struct Wrapper<Value::Symbol> : public Blueprint<Value::Symbol, "Symbol"> {
  //  PUBLIC METHODS  //

  /// @brief Gets the baseline symbol type-class.
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
  static Member::View m_attribute(const Value::Symbol &self, const Value::Symbol &symbol);
};

} // namespace Sabre::Builtins

#endif
