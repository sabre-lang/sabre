#ifndef _SABRE_FLOW_UNREACHABLE_HPP
#define _SABRE_FLOW_UNREACHABLE_HPP

/// Type Includes
#include "sabre/flow/control.hpp"

namespace Sabre::Flow {

/// @brief Unreachable Control Flow.
class Unreachable : public Mixin<Unreachable> {
  //  PROPERTIES  //

  /// @brief Associated control effect.
  Effect m_result;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an unreachable control flow.
   * @param degree                Unreachable degree.
   * @param effect                Optional side-effect.
   */
  explicit constexpr Unreachable(Degree degree, Effect effect = Effect::NONE) : Mixin(degree), m_result(effect) {
    $_ASSERT(unreachable(), "Expected a non-negative reachability value");
  }

protected:
  //  PRIVATE METHODS  //

  /// @brief Gets the associated side-effect.
  inline constexpr Effect m_effect() const noexcept final { return m_result; }
};

} // namespace Sabre::Flow

#endif
