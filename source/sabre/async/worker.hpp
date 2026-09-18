#ifndef _SABRE_ASYNC_WORKER_HPP
#define _SABRE_ASYNC_WORKER_HPP

/// Sabre Includes
#include "sabre/async/future.hpp"
#include "sabre/resource/frame.hpp"
#include "sabre/runtime/executor.hpp"

namespace Sabre::Async {

/// @brief Future Worker Isolate.
class Worker : public Runtime::Executor {
  //  PROPERTIES  //

  /// @brief Underlying resource trace.
  Resource::Trace m_trace = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a future isolate.
   * @param services          Services container.
   * @param future            Future to bind.
   * @param trace             Resource trace.
   */
  explicit Worker(XI::Container *services, const Future &future, const Resource::Trace &trace = {}) :
      Executor(services, future), m_trace(trace) {}

protected:
  //  PRIVATE METHODS  //

  /// @brief Handles launching the runtime.
  void m_execute() final;
};

} // namespace Sabre::Async

#endif
