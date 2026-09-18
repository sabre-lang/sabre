#ifndef _SABRE_RUNTIME_EXECUTOR_HPP
#define _SABRE_RUNTIME_EXECUTOR_HPP

/// Sabre Includes
#include "sabre/runtime/isolate.hpp"

namespace Sabre::Runtime {

/// @brief Executor for Isolates.
struct $_ABSTRACT Executor : public Async::Task<Executor>, public Isolate {
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an isolate executor.
   * @param args                  Arguments to bind.
   */
  template <class... As> explicit Executor(As &&...args) : Isolate(std::forward<As>(args)...) {}

  //  PUBLIC METHODS  //

  /// @brief Allow getting the underlying thread instance.
  inline constexpr Async::Thread *thread() const noexcept final { return Async::Task<Executor>::thread(); }
};

} // namespace Sabre::Runtime

#endif
