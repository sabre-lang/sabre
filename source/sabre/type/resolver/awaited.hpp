#ifndef _SABRE_TYPE_AWAITED_HPP
#define _SABRE_TYPE_AWAITED_HPP

/// Type Includes
#include "sabre/type/utility/transform.hpp"

namespace Sabre::Type::Utility {

/// @brief Awaited Type Resolver.
struct Awaited {
  //  CONSTRUCTORS  //

  /// @brief Default constructor.
  explicit Awaited() = default;

  //  OPERATOR METHODS  //

  /**
   * @brief Handles awaiting a target.
   * @param target                Target to await.
   * @param constraints           Constraints to use.
   */
  Erased operator()(const Erased &target, Constraints *constraints) const noexcept;

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles awaiting a target.
   * @param target                Target to await.
   * @param constraints           Constraints to use.
   */
  Erased m_resolve(const Erased &target, Constraints *constraints) const noexcept;
};

/// @brief Ensure the resolution conversion is valid.
static_assert(std::convertible_to<Awaited, Resolver>);

} // namespace Sabre::Type::Utility

#endif
