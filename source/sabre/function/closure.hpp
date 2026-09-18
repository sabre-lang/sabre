#ifndef _SABRE_FUNCTION_CLOSURE_HPP
#define _SABRE_FUNCTION_CLOSURE_HPP

/// Sabre Includes
#include "sabre/function/environ.hpp"
#include "sabre/function/info.hpp"
#include "sabre/object/common.hpp"

namespace Sabre {

/// @brief Closure Function Attributes.
template <> struct Object::Wrapper<Function::Closure> {
  //  PROPERTIES  //

  /// @brief Shared function information.
  const Function::Info *info = nullptr;

  /// @brief Bound self value.
  Value::Any receiver = Value::Void();

  /// @brief Bound function environment.
  Function::Environ context = Function::Environ();

  //  CONSTRUCTORS  //

  /// @brief Allow default construction.
  explicit Wrapper() = default;

  /**
   * @brief Constructs a closure.
   * @param info              Closure information.
   * @param self              Bound receiver value.
   * @param context            Context to be bound.
   */
  explicit Wrapper(const Function::Info *info) : Wrapper(info, Value::Void()) {}
  explicit Wrapper(const Function::Info *info, const Value::Any &self) : Wrapper(info, self, Value::Void()) {}
  explicit Wrapper(const Function::Info *info, const Value::Any &self, const Value::Any &context) :
      info(info), receiver(self), context(context) {}
};

/// @brief Closure Function Interface.
struct Function::Closure : public Object::Mixin<Function::Closure> {
  //  TYPEDEFS  //

  /// @brief Allow base function access.
  friend struct Function::Any;

  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Mixin::Mixin;

  //  PUBLIC METHODS  //

  /// @brief Gets the arity of a function closure.
  inline constexpr uint64_t arity() const noexcept { return info()->arity(); }

  /// @brief Gets the adicity of a function closure.
  inline constexpr uint64_t adicity() const noexcept { return info()->adicity(); }

  /// @brief Gets the associated closure resource.
  inline constexpr $::URI::View resource() const noexcept { return info()->resource(); }

  /// @brief Gets the baseline function information.
  inline constexpr const Info *info() const noexcept { return m_wrapper()->info; }

  /// @brief Gets the function environment.
  inline constexpr const Environ &context() const { return m_wrapper()->context; }

  /// @brief Gets the bound receiver value.
  inline constexpr Value::Any receiver() const noexcept { return m_wrapper()->receiver; }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handlers yielding closures safely.
   * @param self                          Closure instance.
   * @param yield                         Yield callback.
   */
  static void m_yield(const Closure &self, Globals::Each &yield);
};

} // namespace Sabre

#endif
