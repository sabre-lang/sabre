#ifndef _SABRE_BUILTINS_VOID_HPP
#define _SABRE_BUILTINS_VOID_HPP

/// Sabre Includes
#include "sabre/builtins/wrapper.hpp"

namespace Sabre::Builtins {

/// @brief Void Builtin Traits.
template <> struct Wrapper<Value::Void> : public Define<Value::Void, "Void", Adapter::GLOBALS, Adapter::TYPEDEFS> {
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
   */
  static Value::Any m_globals(Isolate *isolate);
};

} // namespace Sabre::Builtins

#endif
