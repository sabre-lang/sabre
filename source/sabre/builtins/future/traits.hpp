#ifndef _SABRE_BUILTINS_FUTURE_HPP
#define _SABRE_BUILTINS_FUTURE_HPP

/// Sabre Includes
#include "sabre/builtins/wrapper.hpp"

namespace Sabre::Builtins {

/// @brief Tagged Future Builtin Traits.
template <> struct Wrapper<Async::Future> : public Blueprint<Async::Future, "Future"> {
  //  PUBLIC METHODS  //

  /// @brief Gets the generic typing for this builtin.
  static $::Shared::Pointer<Type::Generic> generic();

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
   * @brief Handles looking up value fields.
   * @param self                      Value instance.
   * @param symbol                    Field symbol.
   */
  static Member::View m_attribute(const Async::Future &self, const Value::Symbol &symbol);
};

} // namespace Sabre::Builtins

#endif
