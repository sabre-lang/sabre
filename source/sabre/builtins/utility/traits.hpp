#ifndef _SABRE_BUILTINS_UTILITY_HPP
#define _SABRE_BUILTINS_UTILITY_HPP

/// Sabre Includes
#include "sabre/builtins/wrapper.hpp"

/// Forward Declarations
$_FWD(Sabre::Builtins::Custom, struct Utility)

namespace Sabre::Builtins {

/// @brief Utility Custom Builtin Traits.
template <> struct Wrapper<Custom::Utility> : public Define<Custom::Utility, "Utility", Adapter::TYPEDEFS> {
protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles defining global type definitions.
   * @param globals                     Global type-world.
   */
  static void m_typedefs(Type::World *globals);
};

} // namespace Sabre::Builtins

#endif
