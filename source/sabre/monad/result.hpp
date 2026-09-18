#ifndef _SABRE_MONAD_RESULT_HPP
#define _SABRE_MONAD_RESULT_HPP

/// Sabre Includes
#include "sabre/object/mixin.hpp"

/// Forward Declarations
$_FWD(Sabre::Monad, Result Success(Runtime::Isolate *, const Value::Any &))
$_FWD(Sabre::Monad, Result Failure(Runtime::Isolate *, const Value::Any &))

namespace Sabre {

//  TYPEDEFS  //

/// @brief Result Object Attributes.
template <> struct Object::Wrapper<Monad::Result> {
  //  PROPERTIES  //

  /// @brief Denotes result typing.
  bool success = true;

  /// @brief The result value.
  Value::Any storage = {};

  //  CONSTRUCTORS  //

  /// @brief Constructs an empty result.
  explicit Wrapper() = default;

  /**
   * @brief Constructs a result monad.
   * @param value             Value to bind.
   */
  explicit Wrapper(const Value::Any &value, std::true_type) : success(true), storage(value) {}
  explicit Wrapper(const Value::Any &value, std::false_type) : success(false), storage(value) {}
};

/// @brief Result Monad Interface.
struct Monad::Result : public Object::Mixin<Monad::Result> {
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Mixin::Mixin;

  //  PUBLIC METHODS  //

  /// @brief Denotes what type of result we have.
  inline constexpr bool success() const noexcept { return m_wrapper()->success; }
  inline constexpr bool failure() const noexcept { return !m_wrapper()->success; }

  /// @brief Gets the underlying storage value.
  inline constexpr Value::Any storage() const noexcept { return m_wrapper()->storage; }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handlers yielding results safely.
   * @param self                          Result instance.
   * @param yield                         Yield callback.
   */
  static void m_yield(const Result &self, Globals::Each &yield);
};

} // namespace Sabre

#endif
