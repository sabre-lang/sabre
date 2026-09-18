/// Sabre Includes
#include <sabre/async/service.hpp>
#include <sabre/testing/inspect.hpp>
#include <sabre/testing/service.hpp>

/// Crate Includes
#include "crates/test/source/addon.hpp"

//  PROPERTIES  //

/// @brief The underlying testing addon installer.
SABRE_MM_DYLIB_ADDON(Test, CRATE_XX_TEST_METHODS)

//  ADDON METHODS  //

SABRE_MM_DYLIB_METHOD(Test, case, isolate, args) {
  // ensure testing is enabled before continuing
  if (!m_enabled(isolate)) return Value::Void();

  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1); // get named
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);

  // register without modifiers and return now
  return m_register(isolate, args), Value::Void();
}

SABRE_MM_DYLIB_METHOD(Test, skip, isolate, args) {
  // ensure testing is enabled before continuing
  if (!m_enabled(isolate)) return Value::Void();

  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1); // get named
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);

  // register with the "skip" modifier now
  return m_register(isolate, args)->skip(true), Value::Void();
}

SABRE_MM_DYLIB_METHOD(Test, todo, isolate, args) {
  // ensure testing is enabled before continuing
  if (!m_enabled(isolate)) return Value::Void();

  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1); // get named
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);

  // register with the "todo" modifier now
  return m_register(isolate, args)->todo(true), Value::Void();
}

SABRE_MM_DYLIB_METHOD(Test, bench, isolate, args) {
  // ensure testing is enabled before continuing
  if (!m_enabled(isolate)) return Value::Void();

  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1); // get named
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);

  // prepare the arguments to be used now
  auto name = $::String::Buffer(args.at<String::Any>(0).view());
  auto callback = args.at<Function::Any>(1, Function::Any());

  // get the underlying testing registry
  auto *storage = isolate->service<Testing::Service>()->storage();
  storage->bench(name, m_bind(isolate, callback), m_location(isolate));

  // and declare a success on completion now
  return Value::Void();
}

SABRE_MM_DYLIB_METHOD(Test, suite, isolate, args) {
  // ensure testing is enabled before continuing
  if (!m_enabled(isolate)) return Value::Void();

  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1); // get named
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);

  // prepare the arguments to be used now
  auto name = $::String::Buffer(args.at<String::Any>(0).view());
  auto callback = args.at<Function::Any>(1, Function::Any());

  // get the underlying testing registry
  auto *storage = isolate->service<Testing::Service>()->storage();
  auto *group = storage->group(name, m_location(isolate));

  // bind the setup handler for the group
  group->bind(m_bind(isolate, callback, group));

  // and declare a success on completion
  return Value::Void();
}

//  PRIVATE METHODS  //

bool Sabre::Package::Test::m_enabled(Runtime::Isolate *isolate) {
  return isolate->service<XI::Container>()->exists<Testing::Service>();
}

Sabre::Testing::Case *Sabre::Package::Test::m_register(Runtime::Isolate *isolate, const Function::Args &args) {
  auto name = $::String::Buffer(args.at<String::Any>(0).view());
  auto callback = args.at<Function::Any>(1, Function::Any());
  auto *storage = isolate->service<Testing::Service>()->storage();
  return storage->test(name, m_bind(isolate, callback), m_location(isolate));
}

Sabre::Testing::Callback Sabre::Package::Test::m_bind(Runtime::Isolate *isolate, const Function::Any &callback) {
  auto *services = isolate->service<XI::Container>();
  if (callback.pointer().nullish()) return [](Testing::Runner *) {};
  return [services, callback](Testing::Runner *runner) { m_callback(services, runner, callback); };
}

Sabre::Testing::Callback
Sabre::Package::Test::m_bind(Runtime::Isolate *isolate, const Function::Any &callback, Testing::Group *group) {
  auto *services = isolate->service<XI::Container>();
  if (callback.pointer().nullish()) return [](Testing::Runner *) {};
  return [services, callback, group](Testing::Runner *runner) {
    // resolve the underlying storage instance
    Testing::Service *testing = *services;
    $_UNUSED $_AUTO = testing->storage()->target(group);

    // race the incoming result now
    auto result = m_race(services, runner, callback);
    if (result.has_value()) return; // valid

    // prepare the incoming exception now
    auto exception = result.error().as<Object::Exception>();
    auto message = $::String::Buffer(exception.message().view());
    runner->asserts()->fail(message, Testing::Inspect::location(exception, false));

    // otherwise we want to safely fail now
    throw std::runtime_error(message);
  };
}

void Sabre::Package::Test::m_callback(XI::Container *services, Testing::Runner *runner, const Function::Any &callback) {
  // race the incoming result now
  auto result = m_race(services, runner, callback);
  if (result.has_value()) return;

  // handle incoming panics as failed assertions
  auto exception = result.error().as<Object::Exception>();
  auto location = Testing::Inspect::location(exception, false);
  runner->asserts()->fail(exception.message().view(), location);
}

Sabre::Async::Result
Sabre::Package::Test::m_race(XI::Container *services, Testing::Runner *runner, const Function::Any &callback) {
  // get the underlying async service
  Async::Service *async = *services;

  // prepare the baseline isolate instance now
  auto *isolate = async->isolate();
  auto *thread = isolate->thread();

  // ensure we instance the future to be called
  auto future = isolate->create<Async::Future>(callback);

  // prepare some details for out timer and future
  auto duration = runner->options()->bench.timeout;

  // attempt waiting for the future to complete now
  auto result = future.timeout(thread, duration);

  // check if the result timed out at all now
  auto ignore = result.has_value() || result.error() != Value::Timeout();

  // handle incoming timeouts if necessary to do so now
  return ignore ? result : throw XT::Assert::Timeout();
}

XT::Trivia::Location Sabre::Package::Test::m_location(Runtime::Isolate *isolate) {
  return m_location(isolate->frame(1)); // need parent not current
}

XT::Trivia::Location Sabre::Package::Test::m_location(const Engine::Frame *frame) {
  return $_ASSERT(frame, "Invalid isolate frame"), m_location(frame->backtrace());
}

XT::Trivia::Location Sabre::Package::Test::m_location(const Resource::Trace &backtrace) {
  return Testing::Inspect::location(backtrace);
}
