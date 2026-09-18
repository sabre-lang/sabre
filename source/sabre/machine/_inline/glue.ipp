#ifndef _SABRE_MACHINE_GLUE_IPP
#define _SABRE_MACHINE_GLUE_IPP

/// Sabre Includes
#include "sabre/engine/dispatch.hpp"
#include "sabre/engine/exports.hpp"
#include "sabre/engine/invoke.hpp"
#include "sabre/garbage/lifetimes.hpp"
#include "sabre/machine/constants.hpp"
#include "sabre/machine/frame.hpp"
#include "sabre/machine/service.hpp"
#include "sabre/runtime/isolate.hpp"
#include "sabre/signal/service.hpp"

/// Value Includes
#include "sabre/value/_inline/value.ipp"

namespace Sabre::Machine::Glue {

/**
 * @brief Coordinates logical "or" operations.
 * @param left                  Left-most value.
 * @param right                 Right-most value.
 */
static inline Reference lor(Reference left, Reference right) {
  return Value::Boolean(Value::Cast(left).truthiness() || Value::Cast(right).truthiness());
}

/**
 * @brief Coordinates logical "and" operations.
 * @param left                  Left-most value.
 * @param right                 Right-most value.
 */
static inline Reference land(Reference left, Reference right) {
  return Value::Boolean(Value::Cast(left).truthiness() && Value::Cast(right).truthiness());
}

/**
 * @brief Concatenates two values.
 * @param isolate               Runtime isolate.
 * @param left                  Left-most value.
 * @param right                 Right-most value.
 */
static inline Reference concat(Runtime::Isolate *isolate, Reference left, Reference right) {
  return Engine::Dispatch::concat(isolate, Value::Cast(left), Value::Cast(right)).pointer();
}

/**
 * @brief Handles setting field values.
 * @param isolate               Runtime isolate.
 * @param target                Target to update.
 * @param symbol                Symbol for field.
 */
static inline Reference getter(Runtime::Isolate *isolate, Reference target, const Value::Symbol &symbol) {
  return Engine::Dispatch::getter(isolate, Value::Cast(target), symbol).pointer();
}

/**
 * @brief Handles setting field values.
 * @param isolate               Runtime isolate.
 * @param target                Target to update.
 * @param value                 Value to assign.
 * @param symbol                Symbol for field.
 */
static inline Reference
setter(Runtime::Isolate *isolate, Reference target, Reference value, const Value::Symbol &symbol) {
  return Engine::Dispatch::setter(isolate, Value::Cast(target), Value::Cast(value), symbol).pointer();
}

/**
 * @brief Handles testing string comparisons.
 * @param target                Target to test.
 * @param intern                String intern.
 */
static inline bool compare(Reference target, const String::Intern *intern) {
  auto input = Value::Cast(target); // cast
  if (!input.is<String::Any>()) return false;
  return input.as<String::Any>().compare(intern->view());
}

/**
 * @brief Handles checking subtyping.
 * @param value                 Value to check.
 * @param guard                 Type-cast guard.
 */
static inline Engine::Subtype extends(Reference value, Reference guard) {
  return Engine::Dispatch::extends(Value::Cast(value), Value::Cast(guard));
}

/**
 * @brief Handles matching values.
 * @param value                 Value to check.
 * @param guard                 Type-cast guard.
 */
static inline bool matches(Reference value, Reference guard) {
  return Engine::Dispatch::matches(Value::Cast(value), Value::Cast(guard));
}

/**
 * @brief Handles ensuring subtyping.
 * @param isolate               Runtime isolate.
 * @param value                 Value to check.
 * @param guard                 Type-cast guard.
 */
static inline Reference ensure(Runtime::Isolate *isolate, Reference value, Reference guard) {
  return Engine::Dispatch::ensure(isolate, Value::Cast(value), Value::Cast(guard)).pointer();
}

/**
 * @brief Handles override object attributes.
 * @param isolate               Runtime isolate.
 * @param target                Target to update.
 * @param callback              Attribute callback.
 * @param kind                  Operator kind.
 */
static inline Reference
overrides(Runtime::Isolate *isolate, Reference target, Reference callback, Operator::Kind kind) {
  return Engine::Dispatch::overrides(isolate, Value::Cast(target), Value::Cast(callback), kind).pointer();
}

/**
 * @brief Helper for casting function environs.
 * @param context               Context value.
 */
static inline Function::Environ &envp(Reference &context) { return *reinterpret_cast<Function::Environ *>(&context); }

/**
 * @brief Handles loading upvalues.
 * @param context               Context value.
 * @param slot                  Upvalue slot.
 * @param depth                 Context depth.
 */
static inline Reference ldar(Reference context, uint16_t slot) { return envp(context).load(slot).pointer(); }
static inline Reference ldup(Reference context, uint16_t slot, uint16_t depth) {
  return envp(context).parent(depth).load(slot).pointer();
}

/**
 * @brief Handles storing upvalues.
 * @param context               Context value.
 * @param value                 Value to store.
 * @param slot                  Upvalue slot.
 * @param depth                 Context depth.
 */
static inline void star(Reference &context, Reference v, uint16_t slot) { envp(context).store(slot, Value::Cast(v)); }
static inline void stup(Reference &context, Reference v, uint16_t slot, uint16_t depth) {
  envp(context).parent(depth).store(slot, Value::Cast(v));
}

/**
 * @brief Helper for casting function arguments.
 * @param values                Values to cast.
 */
static inline const Function::Args &argv(const Value::Any *&values) {
  return *reinterpret_cast<const Function::Args *>(&values);
}

/**
 * @brief Handles passthrough invocation.
 * @param isolate               Runtime isolate.
 * @param frame                 Machine frame.
 */
static inline Reference pass(Runtime::Isolate *isolate, const Frame *frame) {
  // get the context to be destructured
  auto *args = frame->argv();
  auto context = frame->context();

  // ensure that are context and arguments are matched
  $_ASSERT(args->self() == frame->self());

  // should safely be able to invoke the call
  return Engine::Invoke::dynamic(isolate, context.load(0), *args).pointer();
}

/**
 * @brief Glues a baseline invocation.
 * @param isolate               Runtime isolate.
 * @param target                Target to call.
 * @param values                Argument values.
 */
static inline Reference invoke(Runtime::Isolate *isolate, Reference target, const Value::Any *values) {
  return Engine::Invoke::dynamic(isolate, Value::Cast(target), argv(values)).pointer();
}

/**
 * @brief Glues a baseline invocation.
 * @param isolate               Runtime isolate.
 * @param info                  Machine info.
 * @param values                Argument values.
 */
static inline Reference recall(Runtime::Isolate *isolate, const Info *info, const Value::Any *values) {
  return Engine::Invoke::inlined(isolate, info, argv(values)).pointer();
}

/**
 * @brief Glues a baseline asynchronous invocation.
 * @param isolate               Runtime isolate.
 * @param target                Target to call.
 * @param values                Argument values.
 */
static inline Reference spawn(Runtime::Isolate *isolate, Reference target, const Value::Any *values) {
  return Engine::Dispatch::spawn(isolate, Value::Cast(target), argv(values)).pointer();
}

/**
 * @brief Handles constructing empty objects.
 * @param isolate               Runtime isolate.
 */
template <class T> static inline Reference empty(Runtime::Isolate *isolate) { return isolate->create<T>().pointer(); }

/**
 * @brief Constructs a string value.
 * @param isolate               Runtime isolate.
 * @param intern                String intern.
 */
static inline Reference string(Runtime::Isolate *isolate, const String::Intern *intern) {
  return String::Any(isolate, *intern).pointer();
}

/**
 * @brief Constructs a closure value.
 * @param isolate               Runtime isolate.
 * @param frame                 Machine frame.
 * @param info                  Function information.
 */
static inline Reference closure(Runtime::Isolate *isolate, const Frame *frame, const Function::Info *info) {
  return isolate->create<Function::Closure>(info, frame->self(), frame->context()).pointer();
}

/**
 * @brief Constructs a closure value.
 * @param isolate               Runtime isolate.
 * @param target                Target to ugrade.
 */
static inline Reference upgrade(Runtime::Isolate *isolate, Reference target) {
  if (!Value::Cast(target).is<Function::Closure>()) return target; // ignore invalid targets if given
  return isolate->service<Service>()->upgrade(isolate, Value::Cast<Function::Closure>(target)).pointer();
}

/**
 * @brief Handles calling a super constructor.
 * @param isolate               Runtime isolate.
 * @param frame                 Machine frame.
 * @param values                Argument values.
 */
static inline Reference super(Runtime::Isolate *isolate, const Frame *frame, const Value::Any *values) {
  auto args = argv(values); // prepare the arguments to be used
  auto instance = (args.self() = frame->self()).as<Object::Instance>();
  return Engine::Dispatch::super(isolate, instance, args).pointer();
}

/**
 * @brief Handles binding constructors.
 * @param isolate               Runtime isolate.
 * @param prototype             Class prototype.
 * @param info                  Function information.
 * @param context               Function environment.
 */
static inline Reference
constructor(Runtime::Isolate *isolate, Reference target, const Function::Info *info, const Frame *frame) {
  return Engine::Dispatch::constructor(isolate, Value::Cast<Object::Class>(target), info, frame->context()).pointer();
}

/**
 * @brief Handles calling a member constructor.
 * @param isolate               Runtime isolate.
 * @param instance              Class instance.
 * @param intern                Member name.
 * @param value                 Member value.
 * @param immutable             Immutable flag.
 */
static inline Reference
member(Runtime::Isolate *isolate, const Frame *frame, const String::Intern *intern, Reference value, bool immutable) {
  auto instance = frame->self().as<Object::Instance>(); // prepare the target value to be assigned to
  return Engine::Dispatch::member(isolate, instance, intern, Value::Cast(value), immutable).pointer();
}

/**
 * @brief Handles creating prototypes.
 * @param isolate               Runtime isolate.
 * @param name                  Name of class.
 * @param shape                 Class shape.
 * @param super                 Super value.
 */
static inline Reference
prototype(Runtime::Isolate *isolate, const String::Intern *name, Shape::Underlying shape, Reference super) {
  return isolate->create<Object::Class>(name->view(), shape, Value::Cast(super)).pointer();
}

/**
 * @brief Constructs an object value.
 * @param isolate               Runtime isolate.
 * @param values                Argument values.
 */
static inline Reference list(Runtime::Isolate *isolate, const Value::Any *values) {
  return isolate->create<Iterable::List>(argv(values).span()).pointer();
}

/**
 * @brief Constructs an object value.
 * @param isolate               Runtime isolate.
 * @param values                Argument values.
 */
static inline Reference object(Runtime::Isolate *isolate, const Value::Any *values) {
  return Engine::Dispatch::object(isolate, argv(values)).pointer();
}

/**
 * @brief Constructs an iterator value.
 * @param isolate               Runtime isolate.
 * @param value                 Potential iterable.
 */
static inline Reference iterator(Runtime::Isolate *isolate, Reference value) {
  return Engine::Dispatch::iterator(isolate, Value::Cast(value)).pointer();
}

/**
 * @brief Constructs an enumeration value.
 * @param isolate               Runtime isolate.
 * @param values                Argument values.
 */
static inline Reference enumeration(Runtime::Isolate *isolate, const Value::Any *values) {
  return Engine::Dispatch::enumeration(isolate, argv(values)).pointer();
}

/**
 * @brief Gets the truthiness of a value.
 * @param target                Target to resolve.
 */
static inline uint64_t truthiness(Reference target) { return Value::Cast(target).truthiness(); }

/**
 * @brief Helpful glue method for printing values.
 * @param value                 Value to print.
 * @param pretty                Prettiness flag.
 */
static inline void print(Reference value, bool pretty) {
  if (pretty) $::Debug::eprintln(Value::Cast(value));
  else $::Debug::eprintln(Pointer::Tagged(value));
}

/**
 * @brief Starts watching a disposable lifetime.
 * @param isolate               Runtime isolate.
 * @param depth                 Opening depth.
 */
static inline void watch(Runtime::Isolate *isolate, size_t depth) { isolate->lifetimes()->open(isolate, depth); }

/**
 * @brief Stops watching a disposable lifetime.
 * @param isolate               Runtime isolate.
 * @param depth                 Closing depth.
 */
static inline Reference ignore(Runtime::Isolate *isolate, size_t depth) {
  return isolate->lifetimes()->close(isolate, depth);
}

/**
 * @brief Assigns a value to be deferred for disposing.
 * @param isolate               Runtime isolate.
 * @param target                Disposable target.
 */
static inline void defer(Runtime::Isolate *isolate, Reference target) {
  isolate->lifetimes()->defer(isolate, Value::Cast(target));
}

/**
 * @brief Handles opening a export scope.
 * @param isolate               Runtime isolate.
 * @param frame                 Machine frame.
 */
static inline Reference open(Runtime::Isolate *isolate, const Frame *frame) {
  return isolate->exports(frame->resource().buffer())->open(isolate).pointer();
}

/**
 * @brief Handles closing a export scope.
 * @param isolate               Runtime isolate.
 * @param frame                 Machine frame.
 */
static inline Reference close(Runtime::Isolate *isolate, const Frame *frame) {
  return isolate->exports(frame->resource().buffer())->close().pointer();
}

/**
 * @brief Handles exposing barrel exports.
 * @param isolate               Runtime isolate.
 * @param frame                 Machine frame.
 * @param value                 Barrel value.
 */
static inline Reference barrel(Runtime::Isolate *isolate, const Frame *frame, Reference value) {
  return Engine::Dispatch::barrel(isolate, frame, Value::Cast<Object::Instance>(value)).pointer();
}

/**
 * @brief Handles importing exports.
 * @param isolate               Runtime isolate.
 * @param frame                 Machine frame.
 * @param intern                Import intern.
 */
static inline Reference import(Runtime::Isolate *isolate, const Frame *frame, const String::Intern *intern) {
  auto trace = frame->backtrace(Resource::Group::IMPORT); // prepare the backtrace
  return isolate->import(intern->view(), frame->resource().body(), trace).pointer();
}

/**
 * @brief Handles exporting values.
 * @param isolate               Runtime isolate.
 * @param frame                 Machine frame.
 * @param value                 Export value.
 * @param intern                Export name.
 */
static inline Reference
expose(Runtime::Isolate *isolate, const Frame *frame, Reference value, const String::Intern *intern) {
  return Engine::Dispatch::expose(isolate, frame, Value::Cast(value), intern).pointer();
}

/**
 * @brief Handles cancelling isolate execution.
 * @param isolate               Runtime isolate.
 */
static inline void cancel(Runtime::Isolate *isolate) { isolate->panic(9000200); }

/**
 * @brief Handles executing a panic.
 * @param isolate               Runtime isolate.
 * @param value                 Value to panic.
 */
static inline void panic(Runtime::Isolate *isolate, Reference value) { isolate->panic(Value::Cast(value)); }

/**
 * @brief Handles aborting the thread.
 * @param isolate               Runtime isolate.
 * @param code                  Given exit-code.
 */
static inline void abort(Runtime::Isolate *isolate, int32_t code) { isolate->thread()->shutdown(code); }

/**
 * @brief Handles raising a signal.
 * @param isolate               Runtime isolate.
 * @param code                  Symbolic signal code.
 */
static inline void raise(Runtime::Isolate *isolate, Signal::Symbolic code) {
  isolate->service<Signal::Service>()->raise(code);
}

/**
 * @brief Handles retrieving globals.
 * @param isolate               Runtime isolate.
 * @param symbol                Global symbol.
 */
static inline Reference global(Runtime::Isolate *isolate, const Value::Symbol &symbol) {
  return isolate->global(symbol).pointer();
}

} // namespace Sabre::Machine::Glue

#endif
