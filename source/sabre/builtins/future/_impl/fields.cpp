/// Sabre Includes
#include "sabre/globals/service.hpp"

/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

//  TYPEDEFS  //

#define SABRE_XX_FIELDS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Field, static Value::Any N(Isolate *, const Args &))
#include "sabre/builtins/future/_defines/fields.def"

//  PROPERTIES  //

static auto s_members = Sabre::Builtins::Storage<Sabre::Async::Future>({
#define SABRE_XX_FIELDS_DEFINE(N, ...) {#N, Sabre::Builtins::Field::N},
#include "sabre/builtins/future/_defines/fields.def"
});

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Builtins::Field::then(Isolate *isolate, const Args &args) {
  // ensure some conditions about the arguments
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, Async::Future, args.self());
  SABRE_MM_ASSERT_TYPEOF(isolate, Function::Any, args[0]);

  // prepare the baseline callback now
  static constexpr auto s_callback = [](Isolate *isolate, const Args &args) {
    // prepare the incoming items now
    auto self = args.self<Async::Future>();
    auto handler = args.at<Function::Any>(0);

    // wait for the current future to finish
    auto result = self.await(isolate);

    // prepare the invocation elements now
    std::span<Value::Any> span = {&result, 1};

    // and attempt mapping our incoming result now
    return result.pointer().okay() ? isolate->invoke(handler, span) : result;
  };

  // construct the cached native to be used now
  auto *globals = isolate->service<Globals::Service>();
  static constexpr auto s_method = "Future.then:callback";
  auto native = globals->internal(isolate, s_method, s_callback);

  // and construct the incoming future instance now
  return isolate->create<Async::Future>(native, args);
}

Sabre::Value::Any Sabre::Builtins::Field::fails(Isolate *isolate, const Args &args) {
  // ensure some conditions about the arguments
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, Async::Future, args.self());
  SABRE_MM_ASSERT_TYPEOF(isolate, Function::Any, args[0]);

  // attempt converting to another future
  static constexpr auto s_callback = [](Isolate *isolate, const Args &args) {
    // prepare the incoming items now
    auto self = args.self<Async::Future>();
    auto handler = args.at<Function::Any>(0);

    // wait for the current future to finish
    auto result = self.await(isolate->thread());

    // if we have a normal result, then return
    if (result.has_value()) return *result;

    // otherwise attempt mapping now
    std::span<Value::Any> span = {&result.error(), 1};
    return isolate->invoke(handler, span); // valid here
  };

  // construct the cached native to be used now
  auto *globals = isolate->service<Globals::Service>();
  static constexpr auto s_method = "Future.fails:callback";
  auto native = globals->internal(isolate, s_method, s_callback);

  // and construct the incoming future instance now
  return isolate->create<Async::Future>(native, args);
}

Sabre::Value::Any Sabre::Builtins::Field::await(Isolate *isolate, const Args &args) {
  // prepare the initial result value
  auto result = args.self();

  // attempt iterating whilst possible to do so
  while (result.is<Async::Future>()) result = result.as<Async::Future>().await(isolate);

  // we now have a non-future result so return
  return result;
}

Sabre::Value::Any Sabre::Builtins::Field::state(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Async::Future, args.self());
  auto status = args.self<Async::Future>().status();
  return Number::Tagged(static_cast<uint8_t>(status));
}

//  PRIVATE METHODS  //

Sabre::Member::View
Sabre::Builtins::Wrapper<Sabre::Async::Future>::m_attribute(const Async::Future &, const Value::Symbol &symbol) {
  return s_members.retrieve(symbol);
}
