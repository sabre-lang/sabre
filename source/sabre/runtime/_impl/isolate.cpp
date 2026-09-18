/// Sabre Includes
#include "sabre/runtime/isolate.hpp"
#include "sabre/async/future.hpp"
#include "sabre/engine/invoke.hpp"
#include "sabre/function/frame.hpp"
#include "sabre/garbage/service.hpp"
#include "sabre/globals/roots.hpp"
#include "sabre/globals/service.hpp"
#include "sabre/import/service.hpp"
#include "sabre/resource/frame.hpp"
#include "sabre/runtime/container.hpp"

/// Builtins Includes
#include "sabre/builtins/function/traits.hpp"

//  CONSTRUCTORS  //

Sabre::Runtime::Isolate::Isolate() : Isolate($::Global::get<Container>()) {}
Sabre::Runtime::Isolate::Isolate(XI::Container *services) : Isolate(services, {}) {}
Sabre::Runtime::Isolate::Isolate(XI::Container *services, const Value::Any &data) :
    m_services(services), m_data(data) {}

//  PUBLIC METHODS  //

void Sabre::Runtime::Isolate::roots(Globals::Each &yield) {
  // yield the baseline details
  yield(m_data), yield(m_exception);

  // and then yield all the handles
  for (auto *handle = m_handles.head(); handle; handle = handle->next()) yield(handle->value());

  // finally start going through the frame roots now
  for (auto *frame : m_frames | std::views::reverse) frame->roots(yield);
}

Sabre::Value::Any Sabre::Runtime::Isolate::global(const Value::Symbol &symbol) {
  return m_services->get<Globals::Service>()->get(this, symbol);
}

const Sabre::String::Intern *Sabre::Runtime::Isolate::intern(const Value::Symbol &symbol) {
  // get the top-most frame value
  auto *head = frame();

  // ensure the underlying frame is valid
  if (head == nullptr) return nullptr;

  // attempt finding the arena reference
  const auto *arena = head->arena();
  if (arena == nullptr) return nullptr;

  auto predicate = [symbol](const String::Intern &intern) { return intern.symbol() == symbol; };
  auto iter = std::ranges::find_if(arena->strings, predicate); // attempt finding now ...
  return iter == arena->strings.cend() ? nullptr : &*iter;     // ... resolve as necessary
}

Sabre::Value::Any Sabre::Runtime::Isolate::invoke(const Value::Any &target, const Function::Args &args) {
  return Engine::Invoke::dynamic(this, target, args);
}

Sabre::Async::Result Sabre::Runtime::Isolate::spawn(const Value::Any &target, const Resource::Trace &trace) {
  return spawn(target, {}, trace);
}

Sabre::Async::Result
Sabre::Runtime::Isolate::spawn(const Value::Any &target, Function::Args &&args, const Resource::Trace &trace) {
  // prepare the incoming frame to be used for the execution of the target
  auto frame = trace.anonymous() ? nullptr : $::Unique::New<Resource::Frame>(this, trace);

  // ensure we have a valid target to be awaited on
  if (target.is<Async::Future>()) return target.as<Async::Future>().await(thread());
  else if (!target.is<Function::Any>()) return std::unexpected(panic(6000201, target.brand()));
  else return create<Async::Future>(target.as<Function::Any>(), args).await(thread());
}

Sabre::Function::Any Sabre::Runtime::Isolate::bind(const Function::Any &callback, const Value::Any &receiver) {
  // prepare the passthrough callback to be used
  auto *info = Builtins::Inspect<Function::Any>::glue();

  // construct a passthrough context
  auto context = Function::Environ(this, 2);

  // bind the contextual details
  context.store(0, receiver);
  context.store(1, callback);

  // and construct the resulting passthrough handler
  return create<Function::Closure>(info, receiver, context);
}

Sabre::Resource::Result Sabre::Runtime::Isolate::resolve(const $::String::View &script, const $::FS::Path &hint) const {
  return service<Import::Service>()->resolve(script, hint);
}

Sabre::Value::Any Sabre::Runtime::Isolate::import(const $::URI::Buffer &resource, const Resource::Trace &trace) {
  auto *result = service<Import::Service>()->import(this, resource, trace);
  return result ? result->await(this) : Value::Failure(); // pre-validate
}

Sabre::Value::Any Sabre::Runtime::Isolate::import(const $::String::View &script, const Resource::Trace &trace) {
  return import(script, $::System::cwd(), trace);
}

Sabre::Value::Any
Sabre::Runtime::Isolate::import(const $::String::View &script, const $::FS::Path &hint, const Resource::Trace &trace) {
  auto resource = resolve(script, hint);
  if (resource) return import(*resource, trace);
  else return panic(8000000, resource.error());
}

Sabre::Engine::Exports *Sabre::Runtime::Isolate::exports(const $::String::View &script, const $::FS::Path &hint) {
  auto resource = resolve(script, hint); // resolve the resource
  return resource.has_value() ? exports(resource.value()) : nullptr;
}

Sabre::Engine::Exports *Sabre::Runtime::Isolate::exports(const $::URI::Buffer &resource) {
  auto *module = service<Import::Service>()->fetch(resource); // fetch module
  if (module == nullptr) return panic(8000102, resource.relative()), nullptr;
  return module->metadata<Module::Phase::EXPORTED>(); // can validly return
}

//  PRIVATE METHODS  //

std::vector<Sabre::Resource::Trace> Sabre::Runtime::Isolate::m_backtrace() {
  // prepare the resulting stack to be used
  auto stack = std::vector<Resource::Trace>();

  // if no frames exists, then construct an empty trace
  if (m_frames.empty()) return stack;

  // determine the current frame size to be used
  auto limit = std::min(m_frames.size(), options()->diagnostics.backtraces);

  // attempt appending each of our available frames to the stack
  for (const auto &[ii, frame] : $::Ranges::Each(std::views::reverse(m_frames))) {
    if (ii > limit) break; // reached limit
    stack.emplace_back(frame->backtrace());
  }

  // and return the resulting backtrace as a list
  return stack;
}

Sabre::Garbage::Lifetimes *Sabre::Runtime::Isolate::m_lifetimes() const noexcept {
  return m_services->get<Garbage::Service>()->lifetimes();
}

Sabre::Handle::Scope Sabre::Runtime::Isolate::m_scope(Engine::Exports *exports) noexcept {
  return exports->open(this), scope();
}

Sabre::Value::Any Sabre::Runtime::Isolate::m_panic(const Object::Exception &exception) noexcept {
  if (m_exception != exception) m_exception = exception;
  return Value::Failure(); // force a failure here
}