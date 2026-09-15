/// Talos Includes
#include "talos/engine/invoke.hpp"
#include "talos/engine/dispatch.hpp"
#include "talos/function/common.hpp"
#include "talos/garbage/lifetimes.hpp"
#include "talos/machine/frame.hpp"
#include "talos/object/instance.hpp"
#include "talos/resource/frame.hpp"
#include "talos/runtime/isolate.hpp"

//  PUBLIC METHODS  //

Talos::Value::Any Talos::Engine::Invoke::dynamic(Isolate *isolate, const Value::Any &target, const Args &args) {
  // handle the incoming object types available
  switch (target.pointer().shape()) {
  case Shape::Lookup<Object::Class>(): return construct(isolate, target.as<Object::Class>(), args);
  case Shape::Lookup<Function::Native>(): return native(isolate, target.as<Function::Native>(), args);
  case Shape::Lookup<Function::Jitted>(): return jitted(isolate, target.as<Function::Jitted>(), args);
  case Shape::Lookup<Function::Closure>(): return closure(isolate, target.as<Function::Closure>(), args);
  default: return isolate->panic(6000200, target.brand()); // type is not callable here at all so we fail
  }
}

Talos::Value::Any Talos::Engine::Invoke::native(Isolate *isolate, const Function::Native &native, const Args &args) {
  // ensure the incoming native is validated here
  if ($_UNLIKELY(!m_validate(isolate, native, args))) return Value::Failure();

  // prepare the trace to be used
  static constexpr auto s_group = Resource::Group::NATIVE;

  // build the resource frame to be used now
  $_UNUSED $_AUTO = Resource::Frame(isolate, Resource::Trace(native.resource(), s_group));

  // and call the underlying native now
  return isolate->thread()->checkpoint(), native.callback()(isolate, args);
}

Talos::Value::Any Talos::Engine::Invoke::jitted(Isolate *isolate, const Function::Jitted &jitted, const Args &args) {
  return m_jitted(isolate, jitted.info(), jitted.context(), args);
}

Talos::Value::Any Talos::Engine::Invoke::closure(Isolate *isolate, const Function::Closure &closure, const Args &args) {
  return m_closure(isolate, closure.info(), closure.context(), args);
}

Talos::Value::Any Talos::Engine::Invoke::inlined(Isolate *isolate, Function::Frame *frame, const Args &args) {
  return m_closure(isolate, frame->info(), frame->context(), args);
}

Talos::Value::Any Talos::Engine::Invoke::inlined(Isolate *isolate, const Machine::Info *info, const Args &args) {
  const auto *frame = isolate->frame()->as<Machine::Frame>();
  return m_jitted(isolate, info, frame->context(), args);
}

Talos::Value::Any Talos::Engine::Invoke::construct(Isolate *isolate, const Object::Class &prototype, const Args &args) {
  // prepare the baseline statics here
  auto symbol = Operator::Attribute::CALL;
  const auto &statics = prototype.statics();

  // if the constructor is missing, the panic
  if (!statics.contains(symbol)) return isolate->panic(6000202, prototype.name());

  // pre-build the instance to be constructed
  auto instance = isolate->create<Object::Instance>(prototype);

  // should safely be able to get the constructor now
  auto ctor = statics.at(symbol)->getter(isolate, instance);

  // prepare the passthrough arguments to be used
  auto passthrough = Args(instance, args.span());

  // attempt calling the baseline constructor now
  auto result = dynamic(isolate, ctor, passthrough);
  return result.pointer().okay() ? instance : result;
}

//  PRIVATE METHODS  //

bool Talos::Engine::Invoke::m_validate(Isolate *isolate, const Function::Any &target, const Args &args) {
  return m_validate(isolate, target.arity(), args);
}

bool Talos::Engine::Invoke::m_validate(Isolate *isolate, const Function::Info *info, const Args &args) {
  return m_validate(isolate, info->arity(), args);
}

bool Talos::Engine::Invoke::m_validate(Isolate *isolate, size_t arity, const Args &args) {
  // ensure the arguments have a suitable sizing now
  if ($_UNLIKELY(args.size() < arity)) return isolate->panic(6000251, arity), false;

  // ensure that the instance has not overflowed now as well
  if ($_LIKELY(!isolate->thread()->stack()->overflowed())) return true;

  // otherwise we declare that the maximum recursion depth occuerd
  return isolate->panic("Maximum recursion depth exceeded"), false;
}

Talos::Function::Environ
Talos::Engine::Invoke::m_initialize(Isolate *isolate, size_t leaked, const Function::Environ &parent) {
  return leaked ? Function::Environ(isolate, leaked, parent) : parent;
}

Talos::Value::Any Talos::Engine::Invoke::m_finalize(Isolate *isolate, const Frame *frame, const Value::Any &result) {
  if (!frame->modes().test(Interrupt::DISPOSE)) return result; // ignore
  return isolate->lifetimes()->close(isolate) ? result : Value::Failure();
}

Talos::Value::Any Talos::Engine::Invoke::m_closure(
    Isolate *isolate, const Function::Info *info, const Function::Environ &context, const Args &args
) {
  // ensure the frame is valid before continuing
  if ($_UNLIKELY(!m_validate(isolate, info, args))) return Value::Failure();

  // attempt resolving our necessary sizes to be used now
  size_t locals = info->locals() + 1;
  size_t vargs = info->shared()->vargs;

  // update the locals size for arguments now
  if (locals > UINT32_MAX) locals = args.size() + 1;

  // prepare a constant for frame stack offsets (eg: +2 for context / self)
  static constexpr size_t s_header = -Function::Offset::STK_OPTR, s_args = s_header + 1;

  // build the function frame and stack to be used for calling
  auto stack = isolate->allocate(locals + s_header);
  auto frame = Function::Frame(isolate, info, stack.data() + s_header);

  // update some items to be used now
  auto argc = std::min(args.size(), vargs);

  // prepare all the stack details to be bound now
  stack[s_header + Function::Offset::STK_OPTR] = Value::Any(info->bytecode().address());
  stack[s_header + Function::Offset::STK_ENVP] = m_initialize(isolate, info->leaked(), context);
  stack[s_header + Function::Offset::STK_SIZE] = Value::Any(Pointer::Underlying(argc));
  stack[s_header + Function::Offset::STK_SELF] = args.self(); // the simplest item to bind

  // fill all the incoming arguments and variadic arguments now
  if (argc) std::memcpy(stack.data() + s_args, args.data(), sizeof(Value::Any) * argc);
  if (vargs != UINT64_MAX) stack[argc = vargs + s_args] = isolate->create<Iterable::List>(args.slice(vargs));

  // enforce a checkpoint before running our handler
  $_ASSERT(isolate->frame() == &frame), isolate->thread()->checkpoint();

  // execute our necessary handler
  return m_finalize(isolate, &frame, Dispatch::tailcall(isolate, &frame));
}

Talos::Value::Any Talos::Engine::Invoke::m_jitted(
    Isolate *isolate, const Machine::Info *info, const Function::Environ &context, const Args &args
) {
  // get the incoming maximum locals count to be used
  auto vargs = info->vargs();
  auto locals = info->locals();

  // construct a passthrough set of arguments for some operations
  auto pass = isolate->allocate(locals);

  // clean the incoming passthrough arguments now
  pass[Function::Offset::ARGS_SIZE] = Number::Zero;

  // rebind our incoming variadic arguments if necessary (safe to do since moving into a list)
  if (vargs != UINT64_MAX) args.data()[vargs] = isolate->create<Iterable::List>(args.span(vargs));

  // construct the underlying stack and frame to be used
  Value::Any stack[Machine::Offset::STK_SIZE] = {};
  auto frame = Machine::Frame(isolate, info, nullptr);

  // define all the stack properties now
  stack[Machine::Offset::STK_OPTR] = Value::Any(0);
  stack[Machine::Offset::STK_ENVP] = m_initialize(isolate, info->leaked(), context);
  stack[Machine::Offset::STK_PASS] = std::bit_cast<Value::Any>(pass.data());
  stack[Machine::Offset::STK_DATA] = std::bit_cast<Value::Any>(args.data() - Function::Offset::ARGS_DATA);

  // enforce a checkpoint before running our handler
  $_ASSERT(isolate->frame() == &frame), isolate->thread()->checkpoint();

  // attempt executing the callback for the machine code
  auto result = info->callback()(isolate, &frame, *frame.stack());

  // execute our necessary handler
  return m_finalize(isolate, &frame, Value::Any(result));
}
