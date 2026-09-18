#ifndef _SABRE_BUILTINS_INSPECT_HPP
#define _SABRE_BUILTINS_INSPECT_HPP

/// Sabre Includes
#include "sabre/builtins/wrapper.hpp"
#include "sabre/forward/globals.hpp"
#include "sabre/operator/kind.hpp"

namespace Sabre::Builtins {

/// @brief Exposes Builtin Functionality.
template <class T> struct Inspect : public Wrapper<T> {
  //  TYPEDEFS  //

  /// @brief Allow the globals service internal access.
  friend class Globals::Service;

  //  PUBLIC METHODS  //

  /**
   * @brief Handles member attributes lookup.
   * @param value                 Parent value.
   * @param symbol                Member symbol.
   */
  static inline constexpr Member::View attribute(const T &value, const Value::Symbol &symbol) {
    if constexpr (!Wrapper<T>::template uses<Adapter::FIELDS>()) return nullptr;
    else return Wrapper<T>::m_attribute(value, symbol); // can safely lookup field
  }

  /**
   * @brief Handles operator attributes lookup.
   * @param value                 Parent value.
   * @param kind                  Operator kind.
   */
  static inline constexpr Member::View attribute(const T &value, Operator::Kind kind) {
    if constexpr (!Wrapper<T>::template uses<Adapter::OPERATORS>()) return nullptr;
    else return Wrapper<T>::m_operator(value, kind); // can safely lookup field
  }

private:
  //  PRIVATE METHODS  //

  /// @brief Gets a suitable factory for generating globals.
  static inline constexpr Globals::Factory m_globals() {
    // prepare the name of the builtin
    static constexpr auto s_name = Wrapper<T>::name();

    // get some of the adapter results that we will use for our output
    static constexpr auto s_globals = Wrapper<T>::template uses<Adapter::GLOBALS>();
    static constexpr auto s_prototype = Wrapper<T>::template uses<Adapter::PROTOTYPE>();

    // show a simple trace for debug purposes about the registration
    if constexpr (s_globals || s_prototype) $_TRACE("Builtins::{0}: Registered Globals", s_name);

    // prepare a suitable factory based on prototypes now
    if constexpr (s_prototype) {
      // need a factory for our prototypes to inherit
      static constexpr auto s_factory = [](auto *isolate) {
        static constexpr auto s_shape = Wrapper<T>::shape();
        return isolate->template create<Object::Class>(s_name, s_shape);
      };

      if constexpr (!s_globals) return s_factory; // default to the baseline factory if no globals given
      else return [](Isolate *isolate) { return Wrapper<T>::m_globals(isolate, s_factory(isolate)); };
    }

    // if we at least have the globals definition available, return that factory
    if constexpr (s_globals && !s_prototype) return Wrapper<T>::m_globals;

    // otherwise always default to an empty value if possible
    return nullptr;
  }

  /**
   * @brief Handles emplacing types into the base world.
   * @param types                 Global types world.
   */
  static inline constexpr void m_typedefs(Type::World *types) {
    if constexpr (Wrapper<T>::template uses<Adapter::TYPEDEFS>()) return Wrapper<T>::m_typedefs(types);
  }
};

} // namespace Sabre::Builtins

#endif
