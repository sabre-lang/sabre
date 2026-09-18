#ifndef _SABRE_TYPE_DEDUCTION_HPP
#define _SABRE_TYPE_DEDUCTION_HPP

/// Sabre Includes
#include "sabre/flow/passable.hpp"
#include "sabre/type/registry.hpp"

namespace Sabre::Type {

/// @brief Type Deduction Result.
struct Deduction {
  //  PROPERTIES  //

  /// @brief Resultant type.
  Erased type = New::any();

  /// @brief Flow control node.
  Control flow = nullptr;

  //  CONSTRUCTORS  //

  /// @brief Constructs a baseline deduction.
  constexpr Deduction() = default;

  /**
   * @brief Constructs a passable deduction.
   * @param type                  Type to pass.
   */
  template <std::derived_from<Unit> T>
  constexpr Deduction(const $::Shared::Pointer<T> &type) : Deduction(type, $::Unique::New<Flow::Passable>()) {}

  /**
   * @brief Constructs a passable deduction.
   * @param type                  Type to pass.
   * @param flow                  Flow to pass.
   */
  constexpr Deduction(const Erased &type, Control &&flow) : type(type), flow(std::move(flow)) {}
};

} // namespace Sabre::Type

#endif
