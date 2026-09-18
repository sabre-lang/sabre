#ifndef _SABRE_TYPE_ASYNCIFY_HPP
#define _SABRE_TYPE_ASYNCIFY_HPP

/// Type Includes
#include "sabre/type/utility/transform.hpp"

namespace Sabre::Type::Utility {

/// @brief Asyncify Type Resolver.
struct Asyncify {
  //  CONSTRUCTORS  //

  /// @brief Default constructor.
  explicit Asyncify() = default;

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
static_assert(std::convertible_to<Asyncify, Resolver>);

} // namespace Sabre::Type::Utility

#endif
