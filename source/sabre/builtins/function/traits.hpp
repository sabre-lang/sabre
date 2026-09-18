#ifndef _SABRE_BUILTINS_FUNCTION_HPP
#define _SABRE_BUILTINS_FUNCTION_HPP

/// Sabre Includes
#include "sabre/builtins/wrapper.hpp"

namespace Sabre::Builtins {

/// @brief Dynamic Function Builtin Traits.
template <> struct Wrapper<Function::Any> : public Blueprint<Function::Any, "Function"> {
  //  PUBLIC METHODS  //

  /// @brief Gets the function formation for "bind" glue.
  static const Function::Info *glue();

  /// @brief Gets the baseline function type-class.
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
  static Member::View m_attribute(const Function::Any &self, const Value::Symbol &symbol);
};

/// @brief Native Builtin Traits.
template <> struct Wrapper<Function::Native> : public Wrapper<Function::Any> {};

/// @brief Jitted Builtin Traits.
template <> struct Wrapper<Function::Jitted> : public Wrapper<Function::Any> {};

/// @brief Closure Builtin Traits.
template <> struct Wrapper<Function::Closure> : public Wrapper<Function::Any> {};

} // namespace Sabre::Builtins

#endif
