/// Sabre Includes
#include "sabre/async/thenable.hpp"
#include "sabre/async/future.hpp"
#include "sabre/runtime/isolate.hpp"

//  PUBLIC METHODS  //

Sabre::Async::Result Sabre::Async::Thenable::await(Thread *thread) noexcept { return Deferred::await(thread); }
Sabre::Value::Any Sabre::Async::Thenable::await(Runtime::Isolate *isolate) noexcept {
  auto result = await(isolate->thread());
  if (result.has_value()) return result.value();
  else return isolate->panic(result.error());
}

bool Sabre::Async::Thenable::resolve(Runtime::Isolate *isolate, const Value::Any &value) noexcept {
  return Deferred::resolve(isolate->thread(), value);
}

bool Sabre::Async::Thenable::reject(Runtime::Isolate *isolate, const Value::Any &exception) noexcept {
  return Deferred::reject(isolate->thread(), exception);
}
