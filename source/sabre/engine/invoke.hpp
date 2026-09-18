#ifndef _SABRE_ENGINE_INVOKE_HPP
#define _SABRE_ENGINE_INVOKE_HPP

/// Sabre Includes
#include "sabre/function/frame.hpp"
#include "sabre/machine/info.hpp"

namespace Sabre::Engine {

/// @brief Handles Function Invocations.
struct Invoke : private $::Ensure::Static {
  //  PUBLIC METHODS  //

  /**
   * @brief Handles invoking the given target.
   * @param isolate                 Runtime isolate.
   * @param target                  Target to invoke.
   * @param args                    Function arguments.
   */
  static Value::Any dynamic(Isolate *isolate, const Value::Any &target, const Args &args = {});
  static Value::Any native(Isolate *isolate, const Function::Native &native, const Args &args = {});
  static Value::Any jitted(Isolate *isolate, const Function::Jitted &jitted, const Args &args = {});
  static Value::Any closure(Isolate *isolate, const Function::Closure &closure, const Args &args = {});
  static Value::Any construct(Isolate *isolate, const Object::Class &prototype, const Args &args = {});

  /**
   * @brief Handles inlining the current frame.
   * @param isolate                 Runtime isolate.
   * @param target                  Target to inline.
   * @param args                    Function arguments.
   */
  static Value::Any inlined(Isolate *isolate, Function::Frame *frame, const Args &args = {});
  static Value::Any inlined(Isolate *isolate, const Machine::Info *info, const Args &args = {});

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles validating any function instance.
   * @param isolate               Runtime isolate.
   * @param arity                 Minimum arguments count.
   */
  static bool m_validate(Isolate *isolate, size_t arity, const Args &args);
  static bool m_validate(Isolate *isolate, const Function::Any &target, const Args &args);
  static bool m_validate(Isolate *isolate, const Function::Info *info, const Args &args);

  /**
   * @brief Handles defining a context for a closure.
   * @param isolate               Runtime isolate.
   * @param leaked                Total upvalues.
   * @param context               Parent context.
   */
  static Function::Environ m_initialize(Isolate *isolate, size_t leaked, const Function::Environ &context);

  /**
   * @brief Handles finalizing results (eg: disposables).
   * @param isolate               Runtime isolate.
   * @param frame                 Invocation frame.
   * @param result                Outgoing result.
   */
  static Value::Any m_finalize(Isolate *isolate, const Frame *frame, const Value::Any &result);

  /**
   * @brief Explicit interpreter dispatching.
   * @param isolate               Runtime isolate.
   * @param info                  Function info.
   * @param context               Closure context.
   * @param args                  Arguments to bind.
   */
  static Value::Any
  m_closure(Isolate *isolate, const Function::Info *info, const Function::Environ &context, const Args &args);

  /**
   * @brief Explicit jitted dispatching.
   * @param isolate               Runtime isolate.
   * @param info                  Machine callee.
   * @param context               Closure context.
   * @param args                  Arguments to bind.
   */
  static Value::Any
  m_jitted(Isolate *isolate, const Machine::Info *info, const Function::Environ &context, const Args &args);
};

} // namespace Sabre::Engine

#endif
