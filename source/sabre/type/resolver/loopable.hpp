#ifndef _SABRE_TYPE_LOOPABLE_HPP
#define _SABRE_TYPE_LOOPABLE_HPP

/// Type Includes
#include "sabre/type/utility/transform.hpp"

namespace Sabre::Type::Utility {

/// @brief Loopable Type Resolver.
class Loopable {
  //  PROPERTIES  //

  /// @brief Wrap with an `Iterator` typing.
  bool m_outer = true;

public:
  //  CONSTRUCTORS  //

  /// @brief Default constructor.
  explicit Loopable(bool outer = true) : m_outer(outer) {}

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
static_assert(std::convertible_to<Loopable, Resolver>);

} // namespace Sabre::Type::Utility

#endif
