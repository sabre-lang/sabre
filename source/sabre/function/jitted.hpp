#ifndef _SABRE_FUNCTION_JITTED_HPP
#define _SABRE_FUNCTION_JITTED_HPP

/// Sabre Includes
#include "sabre/function/environ.hpp"
#include "sabre/machine/info.hpp"
#include "sabre/object/common.hpp"

namespace Sabre {

/// @brief Jitted Function Attributes.
template <> struct Object::Wrapper<Function::Jitted> {
  //  PROPERTIES  //

  /// @brief The compiled machine code information.
  const Machine::Info *info = nullptr;

  /// @brief Bound receiver value.
  Value::Any receiver = Value::Void();

  /// @brief Bound function environment.
  Function::Environ context = Function::Environ();

  //  CONSTRUCTORS  //

  /// @brief Constructs an empty jitted function.
  explicit Wrapper() = default;

  /**
   * @brief Constructs a jitted function.
   * @param info                  Machine callee.
   * @param context               Context to bind.
   */
  explicit Wrapper(const Machine::Info *info) : Wrapper(info, Value::Void()) {}
  explicit Wrapper(const Machine::Info *info, const Value::Any &self) : Wrapper(info, self, Value::Void()) {}
  explicit Wrapper(const Machine::Info *info, const Value::Any &self, const Value::Any &context) :
      info(info), receiver(self), context(context) {}
};

/// @brief Jitted Function Interface.
struct Function::Jitted : public Object::Mixin<Function::Jitted> {
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
  inline constexpr const Machine::Info *info() const noexcept { return m_wrapper()->info; }

  /// @brief Gets the function environment.
  inline constexpr const Environ &context() const { return m_wrapper()->context; }

  /// @brief Gets the bound receiver value.
  inline constexpr Value::Any receiver() const noexcept { return m_wrapper()->receiver; }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handlers yielding jitted functions safely.
   * @param self                          Jitted instance.
   * @param yield                         Yield callback.
   */
  static void m_yield(const Jitted &self, Globals::Each &yield);
};

} // namespace Sabre

#endif
