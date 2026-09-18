#ifndef _SABRE_FUNCTION_NATIVE_HPP
#define _SABRE_FUNCTION_NATIVE_HPP

/// Sabre Includes
#include "sabre/function/callback.hpp"
#include "sabre/object/mixin.hpp"

namespace Sabre {

/// @brief Native Function Attributes.
template <> struct Object::Wrapper<Function::Native> {
  //  PROPERTIES  //

  /// @brief Associated native resource.
  $::URI::Buffer resource = {};

  /// @brief The bound callback value.
  Function::Callback callback = [](Runtime::Isolate *, const Function::Args &) -> Value::Any {
    return Value::Void(); // declare a simple value here as necessary
  };

  //  CONSTRUCTORS  //

  /// @brief Constructs an empty native function.
  explicit Wrapper() = default;

  /**
   * @brief Constructs a native with an "eval:..." description.
   * @param callback              Callback to bind.
   * @param description           Description to use.
   */
  explicit Wrapper(Function::Callback callback, const $::String::View &description) :
      Wrapper(callback, $::URI::Evaluate("{0}()", description)) {}

  /**
   * @brief Constructs a native with an "eval:..." description.
   * @param callback              Callback to bind.
   * @param category              Category to bind.
   * @param field                 Member field.
   */
  explicit Wrapper(Function::Callback callback, const $::String::View &parent, const $::String::View &field) :
      Wrapper(callback, fmt::format("{0}.{1}", parent, field)) {}

  /**
   * @brief Constructs a native function.
   * @param callback              Callback to bind.
   * @param resource              Resource to bind.
   */
  explicit Wrapper(Function::Callback callback, const $::URI::Buffer &resource = {}) :
      resource(resource), callback(callback) {}
};

/// @brief Native Function Interface.
struct Function::Native : public Object::Mixin<Function::Native> {
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Mixin::Mixin;

  //  PUBLIC METHODS  //

  /// @brief Gets the arity of a native function.
  inline constexpr size_t arity() const noexcept { return 0; }

  /// @brief Gets the adicity of a native function.
  inline constexpr size_t adicity() const noexcept { return SIZE_MAX; }

  /// @brief Gets the receiver for a native function (always empty).
  inline constexpr Value::Any receiver() const noexcept { return Value::Void(); }

  /// @brief Gets the underlying callback value.
  inline constexpr Callback callback() const noexcept { return m_wrapper()->callback; }

  /// @brief Gets the associated native resource.
  inline constexpr $::URI::View resource() const noexcept { return m_wrapper()->resource; }
};

} // namespace Sabre

#endif
