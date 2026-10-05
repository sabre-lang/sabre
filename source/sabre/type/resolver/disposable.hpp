#ifndef _SABRE_TYPE_DISPOSABLE_HPP
#define _SABRE_TYPE_DISPOSABLE_HPP

/// Type Includes
#include "sabre/type/utility/transform.hpp"

namespace Sabre::Type::Utility {

/// @brief Disposable Type Resolver.
struct Disposable {
  //  CONSTRUCTORS  //

  /// @brief Default constructor.
  explicit Disposable() = default;

  //  OPERATOR METHODS  //

  /**
   * @brief Handles awaiting a target.
   * @param target                Target to await.
   * @param constraints           Constraints to use.
   */
  Erased operator()(const Erased &target, Constraints *constraints) const noexcept;
};

/// @brief Ensure the resolution conversion is valid.
static_assert(std::convertible_to<Disposable, Resolver>);

} // namespace Sabre::Type::Utility

#endif
