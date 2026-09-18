#ifndef _CRATES_PACKAGE_TEST_HPP
#define _CRATES_PACKAGE_TEST_HPP

/// Addon Includes
#include <sabre/dylib/_inline/dylib.ipp>

//  X-MACROS  //

#define CRATE_XX_TEST_METHODS(X) \
  X(case)                        \
  X(skip)                        \
  X(todo)                        \
  X(bench)                       \
  X(suite)

//  NAMESPACES  //

namespace Sabre::Package {

/// @brief Testing Package Addon.
struct Test : public Dylib::Mixin<"test"> {
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a testing library.
   * @param isolate                 Runtime isolate.
   * @param exports                 Addon exports.
   */
  explicit Test(Runtime::Isolate *isolate, Dylib::Exports &exports);

private:
  //  PRIVATE METHODS  //

  CRATE_XX_TEST_METHODS(SABRE_MM_DYLIB_DEFINE)

  /**
   * @brief Handles validating testing is enabled.
   * @param isolate                 Runtime isolate.
   */
  static bool m_enabled(Runtime::Isolate *isolate);

  /**
   * @brief Handles registering tests.
   * @param isolate                 Runtime isolate.
   * @param args                    Function arguments.
   */
  static Testing::Case *m_register(Runtime::Isolate *isolate, const Function::Args &args);

  /**
   * @brief Resolves testing callbacks.
   * @param isolate                 Runtime isolate.
   * @param callback                Callback to bind.
   * @param group                   Optional target group.
   */
  static Testing::Callback m_bind(Runtime::Isolate *isolate, const Function::Any &callback);
  static Testing::Callback m_bind(Runtime::Isolate *services, const Function::Any &callback, Testing::Group *group);

  /**
   * @brief Handles running testing callbacks.
   * @param services                Sevices container.
   * @param runner                  Testing runner.
   * @param callback                Callback to execute.
   */
  static void m_callback(XI::Container *services, Testing::Runner *runner, const Function::Any &callback);

  /**
   * @brief Handles racing a callback.
   * @param services                Sevices container.
   * @param runner                  Testing runner.
   * @param callback                Callback to execute.
   */
  static Async::Result m_race(XI::Container *services, Testing::Runner *runner, const Function::Any &callback);

  /**
   * @brief Converts an isolate into a location.
   * @param isolate                 Runtime isolate.
   */
  static XT::Trivia::Location m_location(Runtime::Isolate *isolate);
  static XT::Trivia::Location m_location(const Engine::Frame *frame);
  static XT::Trivia::Location m_location(const Resource::Trace &trace);
};

} // namespace Sabre::Package

#endif
