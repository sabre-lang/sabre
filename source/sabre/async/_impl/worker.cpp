/// Sabre Includes
#include "sabre/async/worker.hpp"

//  PRIVATE METHODS  //

void Sabre::Async::Worker::m_execute() {
  // prepare a local scoping to be used
  auto local_scope = scope();

  // get the details about the future instance
  auto local_future = local_scope(m_data.as<Async::Future>());
  if (local_future->m_wrapper()->worker == nullptr) return;

  // ensure the worker is the same as the current one
  $_ASSERT(local_future->m_wrapper()->worker == this);

  // prepare a suitable trace to be used if needed
  $_UNUSED $_AUTO = m_trace.anonymous() ? nullptr : $::Unique::New<Resource::Frame>(this, m_trace);

  // destructure the callback and details to be used
  auto *local_thenable = local_future->thenable();
  auto local_callback = local_scope(local_future->callback());

  // attempt running the future callback now
  const auto &local_args = local_future->args();
  auto retval = invoke(*local_callback, local_args);

  // detach the underlying worker instance now
  local_future->m_wrapper()->worker = nullptr;

  // declare the details about the instance now
  if (retval.pointer().okay()) local_thenable->resolve(this, retval);
  else local_thenable->reject(this, m_exception); // failure here now
}
