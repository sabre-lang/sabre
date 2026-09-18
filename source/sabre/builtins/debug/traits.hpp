#ifndef _SABRE_BUILTINS_DEBUG_HPP
#define _SABRE_BUILTINS_DEBUG_HPP

/// Sabre Includes
#include "sabre/builtins/wrapper.hpp"

/// Forward Declarations
$_FWD(Sabre::Builtins::Custom, struct Debug)

namespace Sabre::Builtins {

/// @brief Custom Debug Builtin Traits.
template <> struct Wrapper<Custom::Debug> : public Define<Custom::Debug, "Debug", Adapter::GLOBALS, Adapter::TYPEDEFS> {
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
