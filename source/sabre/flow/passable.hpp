#ifndef _SABRE_FLOW_PASSABLE_HPP
#define _SABRE_FLOW_PASSABLE_HPP

/// Type Includes
#include "sabre/flow/control.hpp"

namespace Sabre::Flow {

/// @brief Passable Control Flow.
struct Passable : public Mixin<Passable> {
  //  CONSTRUCTORS  //

  /// @brief Constructs a passable flow state.
  explicit constexpr Passable() : Mixin(-1) {}

protected:
  //  PRIVATE METHODS  //

  /// @brief Passable flows have no decernable effect.
  inline constexpr Effect m_effect() const noexcept final { return Effect::NONE; }
};

} // namespace Sabre::Flow

#endif
