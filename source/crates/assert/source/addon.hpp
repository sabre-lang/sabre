#ifndef _CRATES_PACKAGE_ASSERT_HPP
#define _CRATES_PACKAGE_ASSERT_HPP

/// Sabre Includes
#include <sabre/testing/service.hpp>

/// Addon Includes
#include <sabre/dylib/_inline/dylib.ipp>

//  X-MACROS  //

#define CRATE_XX_ASSERT_METHODS(X) \
  X(fail)                          \
  X(okay)                          \
  X(truthy)                        \
  X(falsey)                        \
  X(exists)                        \
  X(panics)                        \
  X(equals)                        \
  X(differs)

//  NAMESPACES  //

namespace Sabre::Package {

/// @brief Assertion Package Addon.
struct Assert : public Dylib::Mixin<"assert"> {
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an assertion library.
   * @param isolate               Runtime isolate.
   * @param exports               Addon exports.
   */
  explicit Assert(Runtime::Isolate *isolate, Dylib::Exports &exports);

private:
  //  PRIVATE METHODS  //

  CRATE_XX_ASSERT_METHODS(SABRE_MM_DYLIB_DEFINE)

  /**
   * @brief Handles testing assertions.
   * @param isolate               Runtime isolate.
   * @param args                  Function arguments.
   * @param state                 Assertion state.
   * @param method                Method to check.
   */
  static Value::Any m_test(Runtime::Isolate *isolate, const Function::Args &args, bool, const $::String::View &method);

  /**
   * @brief Handles rethrowing an exception.
   * @param isolate               Runtime isolate.
   * @param exception             Exception to throw.
   */
  static Value::Any m_panic(Runtime::Isolate *isolate, const Object::Exception &exception);

  /**
   * @brief Handles throwing an exception.
   * @param isolate               Runtime isolate.
   * @param code                  Diagnostic code.
   * @param args                  Exception arguments.
   */
  template <class... As>
  static inline Value::Any m_panic(Runtime::Isolate *isolate, Diagnostic::Code code, As &&...args) {
    auto category = Diagnostic::Inspect::category(code); // prepare the items
    auto message = Diagnostic::Inspect::format(code, std::forward<As>(args)...);
    return m_panic(isolate, isolate->create<Object::Exception>(category, message));
  }
};

} // namespace Sabre::Package

#endif
