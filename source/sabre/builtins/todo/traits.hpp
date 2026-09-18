#ifndef _SABRE_BUILTINS_TODO_HPP
#define _SABRE_BUILTINS_TODO_HPP

/// Sabre Includes
#include "sabre/builtins/wrapper.hpp"

/// Forward Declarations
$_FWD(Sabre::Builtins::Custom, struct Todo)

namespace Sabre::Builtins {

/// @brief Todo Custom Builtin Traits.
template <> struct Wrapper<Custom::Todo> : public Define<Custom::Todo, "Todo", Adapter::GLOBALS, Adapter::TYPEDEFS> {
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

  /**
   * @brief Handles executing panics for the "Todo" builtin.
   * @param isolate                   Runtime isolate.
   * @param args                      Function arguments.
   */
  static Value::Any m_panic(Isolate *isolate, const Function::Args &args);
};

} // namespace Sabre::Builtins

#endif
