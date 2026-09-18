#ifndef _SABRE_FLOW_CONTROL_HPP
#define _SABRE_FLOW_CONTROL_HPP

/// Sabre Includes
#include "sabre/flow/effect.hpp"

namespace Sabre::Flow {

/// @brief Flow Reachability Degree Type.
using Degree = int32_t;

/// @brief Flow State.
class $_ABSTRACT Control : public $::RTTI::Dynamic {
  //  PROPERTIES  //

  /// @brief Current reachability depth.
  Degree m_degree;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a suitable state.
   * @param degree            Reachability degree.
   */
  constexpr Control(Degree degree) : m_degree(degree) {}

  /// @brief Allow virtual destruction.
  virtual ~Control() = default;

  //  PUBLIC METHODS  //

  /// @brief Gets the current degree type.
  inline constexpr Degree degree() const noexcept { return m_degree; }

  /// @brief Denotes if the control-flow is passable.
  inline constexpr bool passable() const noexcept { return m_degree < 0; }

  /// @brief Denotes if the control-flow is unreachable.
  inline constexpr bool unreachable() const noexcept { return m_degree > -1; }

  /// @brief Handles getting the current control-flow degree.
  inline constexpr Effect effect() const noexcept { return m_effect(); }

protected:
  //  PRIVATE METHODS  //

  /// @brief Gets the abstracted effect value.
  virtual Effect m_effect() const noexcept = 0;
};

/// @brief Control Inheritance Abstraction.
template <class T> using Mixin = $::RTTI::Mixin<T, Control>;

} // namespace Sabre::Flow

#endif
