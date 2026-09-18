/// Sabre Includes
#include "sabre/async/service.hpp"
#include "sabre/async/worker.hpp"
#include "sabre/globals/roots.hpp"
#include "sabre/runtime/container.hpp"

//  CONSTRUCTORS  //

Sabre::Object::Wrapper<Sabre::Async::Future>::Wrapper(Runtime::Isolate *callee, const Function::Any &callback) :
    Wrapper(callee, callback, {}) {}

Sabre::Object::Wrapper<Sabre::Async::Future>::Wrapper(
    Runtime::Isolate *isolate, const Function::Any &callback, Function::Args &&args
) : args(std::move(args)), callback(callback) {
  m_initialize(isolate);
}

Sabre::Object::Wrapper<Sabre::Async::Future>::Wrapper(
    Runtime::Isolate *isolate, const Function::Any &callback, const Function::Args &args
) : args(args), callback(callback) {
  m_initialize(isolate);
}

//  PUBLIC METHODS  //

Sabre::Async::Result Sabre::Async::Future::timeout(Thread *thread, const $::Chrono::Duration &duration) const noexcept {
  auto timer = m_timeout(duration);
  auto result = thenable()->await(thread);
  if (timer == nullptr) return result;
  if (timer->cancel(thread)) return result;
  return std::unexpected(Value::Timeout());
}

Sabre::Value::Any
Sabre::Async::Future::timeout(Runtime::Isolate *isolate, const $::Chrono::Duration &duration) const noexcept {
  auto timer = m_timeout(duration);
  auto value = thenable()->await(isolate);
  if (timer == nullptr) return value;
  if (timer->cancel(isolate->thread())) return value;
  return isolate->panic(6000101);
}

Sabre::Async::Result Sabre::Async::Future::await(Thread *thread) const noexcept { return thenable()->await(thread); }
Sabre::Value::Any Sabre::Async::Future::await(Runtime::Isolate *isolate) const noexcept {
  return thenable()->await(isolate);
}

//  PRIVATE METHODS  //

void Sabre::Object::Wrapper<Sabre::Async::Future>::m_initialize(Runtime::Isolate *isolate) {
  // prepare the services to be used now
  auto *services = isolate->service<XI::Container>();
  auto *async = isolate->service<Async::Service>();

  // get the associated address now
  auto pointer = std::bit_cast<Pointer::Underlying>(this);
  pointer |= static_cast<Pointer::Underlying>(Pointer::Kind::OBJ);

  auto local_scope = isolate->scope(); // prepare scoping
  auto local_future = local_scope(Async::Future(pointer));

  auto *frame = isolate->frame(); // prepare the resource trace
  auto anonymous = frame == nullptr || callback.resource().anonymous();
  auto trace = anonymous ? Resource::Trace() : frame->backtrace(); // build trace

  // and attempt scheduling the future now
  auto *processor = isolate->thread()->worker()->processor();
  worker = async->spawn<Async::Worker>(processor, services, *local_future, trace);
}

void Sabre::Async::Future::m_yield(const Future &self, Globals::Each &yield) {
  // get the wrapper now
  auto *attributes = self.m_wrapper();

  // bind all the underlying values
  yield(attributes->args.self());
  yield(attributes->args.span());

  // thenables are conditional here
  if (attributes->thenable) yield(attributes->thenable->immediate());
}

Sabre::Async::Timeout Sabre::Async::Future::m_timeout(const $::Chrono::Duration &duration) const noexcept {
  // attempt getting the underlying worker
  auto *isolate = m_wrapper()->worker;
  if (isolate == nullptr) return nullptr;

  // prepare the async handler now
  auto *async = isolate->service<Async::Service>();

  // prepare the callback to be executed
  auto callback = [isolate](Thread *, Processor *) { isolate->interrupt(); };

  // finally attempt constructing the timeout now
  return async->timeout(duration, std::move(callback));
}
