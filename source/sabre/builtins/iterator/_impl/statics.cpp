/// Sabre Includes
#include "sabre/globals/service.hpp"

/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

/// Forward Declarations
$_FWD(Sabre::Builtins::Static, static Value::Any yield(Isolate *, const Args &))

//  TYPEDEFS  //

#define SABRE_XX_STATICS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Static, static Value::Any N(Isolate *, const Args &))
#include "sabre/builtins/iterator/_defines/statics.def"

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Builtins::Static::yield(Isolate *isolate, const Args &args) {
  // ensure some valid conditions firstly
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, Object::Instance, args.self());

  auto self = args.self(); // get details
  auto *descriptor = self.attribute("value");
  $_ASSERT(descriptor != nullptr); // check

  // attempt updating the incoming value now
  return descriptor->setter(isolate, self, args[0]);
}

Sabre::Value::Any Sabre::Builtins::Static::empty(Isolate *isolate, const Args &) {
  return isolate->create<Iterable::Iterator>();
}

Sabre::Value::Any Sabre::Builtins::Static::from(Isolate *isolate, const Args &args) {
  // ensure we have been given a suitable set of arguments
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);

  auto *descriptor = args[0].attribute(Operator::Kind::ITER); // pull out the iterable
  if (descriptor == nullptr) descriptor = args[0].attribute(Operator::Attribute::ITER);
  auto attribute = descriptor ? descriptor->getter(isolate, args[0]) : Value::Missing();
  return attribute.pointer().okay() ? attribute : isolate->panic(6000502, args[0].brand());
}

Sabre::Value::Any Sabre::Builtins::Static::range(Isolate *isolate, const Args &args) {
  // prepare the basic interval details
  auto interval = Iterable::Deduce::interval(isolate, args);
  if (!interval.has_value()) return Value::Failure();

  // attempt preparing the list containing iterable values now
  std::vector<Value::Any> passthrough = {
      Number::Tagged(interval->start()),
      Number::Tagged(interval->stop()),
      Number::Tagged(interval->step()),
  };

  // construct a suitable range iterator now
  Iterable::Callback<Iterable::List> callback = [](auto *, const Iterable::List &self, size_t) -> Value::Any {
    // pull out the context values being used now
    Number::Floating current = self.get(0).as<Number::Tagged>();
    Number::Floating condition = self.get(1).as<Number::Tagged>();
    Number::Floating step = self.get(2).as<Number::Tagged>();

    // we handle our condition based on the step
    if (step > 0 ? current >= condition : current <= condition) return Value::Sentinel();

    // update the current initial value being used
    return self.set(0, Number::Tagged(current + step)), Number::Tagged(current);
  };

  // and construct the resulting iterator now
  return isolate->create<Iterable::Iterator>(isolate->create<Iterable::List>(passthrough), std::move(callback));
}

Sabre::Value::Any Sabre::Builtins::Static::dynamic(Isolate *isolate, const Args &args) {
  // ensure we have been given a suitable set of arguments
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, Function::Any, args[0]);

  // pull out the incoming function now
  auto generator = args.at<Function::Any>(0);

  // create at context to be used for our iterator callback (helps bind all GC references together)
  auto context = isolate->create<Object::Instance>();

  // prepare the incoming yield handler so it is cached only once
  auto *globals = isolate->service<Globals::Service>();
  static constexpr auto s_method = "Iterator.dynamic:yield";
  auto yield = globals->internal(isolate, s_method, Static::yield);

  // we bind the context with the necessary properties for iteration
  context.fields().emplace("next", Member::Factory::reference(generator));
  context.fields().emplace("value", Member::Factory::reference(Value::Void()));
  context.fields().emplace("yield", Member::Factory::reference(isolate->bind(yield, context)));

  // prepare the callback to be used for the iterator
  Iterable::Callback<Object::Instance> callback = [](auto *isolate, Object::Instance self, size_t idx) -> Value::Any {
    // prepare the available properties contained
    auto next = self.attribute("next")->getter(isolate, self);
    auto yield = self.attribute("yield")->getter(isolate, self);

    // prepare the arguments to be used now
    std::vector<Value::Any> args = {Number::Tagged(idx), yield};

    // attempt getting the next handler now
    auto done = isolate->invoke(next, Function::Args(args));

    // check whether or not the operation has now ended
    if (!done.pointer().okay()) return Value::Failure();
    else if (done.truthiness()) return Value::Sentinel();
    else return self.attribute("value")->getter(isolate, self);
  };

  // and now we want to rebuild the iterator as necessary
  return isolate->create<Iterable::Iterator>(context, std::move(callback));
}

//  PRIVATE METHODS  //

Sabre::Value::Any
Sabre::Builtins::Wrapper<Sabre::Iterable::Iterator>::m_globals(Isolate *isolate, const Object::Class &self) {
#define SABRE_XX_STATICS_DEFINE(N, ...)                                                \
  self.statics().emplace(#N, Member::Factory::native(isolate, Static::N, name(), #N));
#include "sabre/builtins/iterator/_defines/statics.def"

  // and return the resulting instance
  return self;
}
