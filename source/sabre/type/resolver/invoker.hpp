#ifndef _SABRE_TYPE_INVOKER_HPP
#define _SABRE_TYPE_INVOKER_HPP

/// Type Includes
#include "sabre/type/compound/callable.hpp"
#include "sabre/type/utility/transform.hpp"

namespace Sabre::Type::Utility {

/// @brief Invocation Type Resolver.
class Invoker {
  //  PROPERTIES  //

  /// @brief Denotes if asynchronous.
  bool m_async = false;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a function invoker.
   * @param async                 Asynchronous flag.
   */
  explicit Invoker(bool async = false) : m_async(async) {}

  //  OPERATOR METHODS  //

  /**
   * @brief Handles instantiating the target.
   * @param target                Target to be instantiated.
   */
  Erased operator()(const Erased &target, Constraints *constraints) const noexcept;

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles binding function parameters.
   * @param target                Target to bind.
   */
  Erased m_bind(const Erased &target) const noexcept;

  /**
   * @brief Handles resolving return-values.
   * @param callable              Callable to resolve.
   */
  Erased m_returns(const $::Shared::Pointer<Callable> &callable) const noexcept;
};

/// @brief Ensure the resolution conversion is valid.
static_assert(std::convertible_to<Invoker, Resolver>);

} // namespace Sabre::Type::Utility

#endif
