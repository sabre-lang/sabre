#ifndef _SABRE_BUILTINS_LIST_HPP
#define _SABRE_BUILTINS_LIST_HPP

/// Sabre Includes
#include "sabre/builtins/wrapper.hpp"

namespace Sabre::Builtins {

/// @brief Tagged List Builtin Traits.
template <> struct Wrapper<Iterable::List> : public Blueprint<Iterable::List, "List", Adapter::OPERATORS> {
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
  static Member::View m_attribute(const Iterable::List &self, const Value::Symbol &symbol);

  /**
   * @brief Handles looking up operator methods.
   * @param self                      Value instance.
   * @param kind                      Operator kind.
   */
  static Member::View m_operator(const Iterable::List &self, Operator::Kind kind);
};

} // namespace Sabre::Builtins

#endif
