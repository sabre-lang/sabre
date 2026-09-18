/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

//  TYPEDEFS  //

#define SABRE_XX_STATICS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Static, static Value::Any N(Isolate *, const Args &))
#include "sabre/builtins/result/_defines/statics.def"

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Builtins::Static::wrap(Isolate *isolate, const Args &args) {
  // validate the incoming results now
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, Async::Future, args[0]);

  // prepare an accumulative result value
  Async::Result result = std::unexpected(Value::Failure());

  // attempt resolve a result whilst possible
  for (auto future = args[0]; future.is<Async::Future>(); future = *result) {
    result = future.as<Async::Future>().await(isolate->thread());
    if (!result.has_value()) break; // break on invalid results
  }

  // if the result return validly, then wrap
  if (result) return Monad::Success(isolate, *result);
  else return Monad::Failure(isolate, result.error());
}

Sabre::Value::Any Sabre::Builtins::Static::okay(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  return Monad::Success(isolate, args[0]);
}

Sabre::Value::Any Sabre::Builtins::Static::error(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  return Monad::Failure(isolate, args[0]);
}

//  PRIVATE METHODS  //

Sabre::Value::Any
Sabre::Builtins::Wrapper<Sabre::Monad::Result>::m_globals(Isolate *isolate, const Object::Class &self) {
#define SABRE_XX_STATICS_DEFINE(N, ...)                                                \
  self.statics().emplace(#N, Member::Factory::native(isolate, Static::N, name(), #N));
#include "sabre/builtins/result/_defines/statics.def"

  // and return the resulting instance
  return self;
}
