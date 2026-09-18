/// Sabre Includes
#include "sabre/garbage/lifetimes.hpp"
#include "sabre/engine/frame.hpp"
#include "sabre/function/common.hpp"
#include "sabre/object/exception.hpp"
#include "sabre/runtime/isolate.hpp"

//  PUBLIC METHODS  //

void Sabre::Garbage::Lifetimes::open(Runtime::Isolate *isolate) { m_open(isolate).emplace_back(Scope()); }
void Sabre::Garbage::Lifetimes::open(Runtime::Isolate *isolate, size_t depth) {
  // get the available stacks for this frame
  auto &stack = m_open(isolate);

  // construct scopes whilst possible
  while (stack.size() <= depth) stack.emplace_back(Scope());
}

bool Sabre::Garbage::Lifetimes::close(Runtime::Isolate *isolate) { return close(isolate, 0); }
bool Sabre::Garbage::Lifetimes::close(Runtime::Isolate *isolate, size_t depth) {
  // get the baseline frame as a key
  const auto *frame = isolate->frame();

  // stop early when possible
  if (!m_stacks.contains(frame)) return true;

  // get the incoming stack now
  auto &stack = m_stacks.at(frame);

  // get the initial exception value
  auto exception = isolate->exception();

  // attempt destructing scopes until depth
  for (; stack.size() > depth; stack.pop_back()) {
    if (m_dispose(isolate, stack.back())) continue;
    if (depth == 0) m_stacks.erase(frame); // clear
    return m_suppress(isolate, exception), false;
  }

  // always run a post-condition to clear
  if (depth == 0) m_stacks.erase(frame);

  // validly removed our stacks
  return true;
}

void Sabre::Garbage::Lifetimes::defer(Runtime::Isolate *isolate, const Value::Any &value) {
  // get the baseline frame as a key
  const auto *frame = isolate->frame();

  // ensure the incoming stack exists now
  $_ASSERT(m_stacks.contains(frame), "Missing disposable stack");
  $_ASSERT(m_stacks.at(frame).size(), "Disposable stack is empty");

  // get the available stack for this frame and emplace
  m_stacks.at(frame).back().emplace_back(value);
}

//  PRIVATE METHODS  //

Sabre::Garbage::Lifetimes::Stack &Sabre::Garbage::Lifetimes::m_open(Runtime::Isolate *isolate) {
  auto *frame = isolate->frame(); // get frame
  frame->interrupt(Engine::Interrupt::DISPOSE);
  return m_stacks[frame]; // resolve frame stacks
}

bool Sabre::Garbage::Lifetimes::m_dispose(Runtime::Isolate *isolate, Scope &values) {
  auto predicate = [&](const Value::Any &value) -> bool { return m_dispose(isolate, value); };
  return std::ranges::all_of(values | std::views::reverse, predicate); // attempt compressing
}

bool Sabre::Garbage::Lifetimes::m_dispose(Runtime::Isolate *isolate, const Value::Any &value) {
  // attempt finding the disposable handler now
  auto *descriptor = value.attribute(Operator::Attribute::DISP);

  // if the attribute is missing, then we do nothing
  if (descriptor == nullptr) return true;

  // since the attribute exists, attempt finding now
  auto attribute = descriptor->getter(isolate, value);

  // fail if the attribute is not a function
  if (!attribute.is<Function::Any>()) return isolate->panic(6001000), false;

  // should be able to safely execute our incoming disposal handler
  return isolate->invoke(attribute, {value}).pointer().okay();
}

void Sabre::Garbage::Lifetimes::m_suppress(Runtime::Isolate *isolate, const Value::Any &exception) {
  if (!exception.is<Object::Exception>()) return;
  m_suppress(isolate, exception.as<Object::Exception>());
}

void Sabre::Garbage::Lifetimes::m_suppress(Runtime::Isolate *isolate, const Object::Exception &exception) {
  // denote that the original exception is "suppressed"
  auto message = Diagnostic::Inspect::message(6001001);

  // get the current exception value from the isolate
  auto current = isolate->exception().as<Object::Exception>();

  // update the underlying message to be used
  exception.message() = String::Any(isolate, message);

  // update the trace being used by the exception
  static auto s_resource = $::URI::Evaluate("Operator.dispose()");
  static auto s_trace = Resource::Trace(s_resource, Resource::Group::NATIVE);
  exception.trace().insert(exception.trace().begin(), s_trace);
  $::Ranges::Prepend(exception.trace(), current.trace());

  // and finally update the exception instance
  isolate->panic(exception);
}
