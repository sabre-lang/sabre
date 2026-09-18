/// Sabre Includes
#include <sabre/testing/inspect.hpp>

/// Crate Includes
#include "crates/assert/source/addon.hpp"

//  PROPERTIES  //

/// @brief The underlying assertion addon installer.
SABRE_MM_DYLIB_ADDON(Assert, CRATE_XX_ASSERT_METHODS)

/// @brief Explicit failed string.
static auto g_failed = Sabre::String::Small("Failed");

//  ADDON METHODS  //

SABRE_MM_DYLIB_METHOD(Assert, okay, isolate, args) { return m_test(isolate, args, true, "okay"); }
SABRE_MM_DYLIB_METHOD(Assert, fail, isolate, args) { return m_panic(isolate, 7000000, args.at(0, g_failed)); }

SABRE_MM_DYLIB_METHOD(Assert, truthy, isolate, args) { return m_test(isolate, args, true, "falsey"); }
SABRE_MM_DYLIB_METHOD(Assert, falsey, isolate, args) { return m_test(isolate, args, false, "truthy"); }

SABRE_MM_DYLIB_METHOD(Assert, exists, isolate, args) {
  // ensure we immediately fail if there are no arguments
  if (args.empty()) return isolate->panic(7000002, "exists");

  // ensure that the value is void now
  if (!args.at(0).is<Value::Void>()) return Value::Void();

  // get the incoming message to be shown now
  auto message = args.at<Value::Any>(1, Value::Void());

  // otherwise we resolve a suitable error message
  if (message.is<String::Any>()) return m_panic(isolate, 7000000, message);
  return m_panic(isolate, 7000001, Value::Inspect<Value::Void>::name());
}

SABRE_MM_DYLIB_METHOD(Assert, equals, isolate, args) {
  // ensure we have the correct number of arguments
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 2);

  // validate the incoming arguments now
  auto actual = args.at(0), expected = args.at(1);
  if (actual == expected) return Value::Void();

  // get the incoming message to be shown now
  auto message = args.at<Value::Any>(2, Value::Void());

  // otherwise we resolve a suitable error message
  return message.is<String::Any>() ? m_panic(isolate, 7000000, message) : m_panic(isolate, 7000003);
}

SABRE_MM_DYLIB_METHOD(Assert, differs, isolate, args) {
  // ensure we have the correct number of arguments
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 2);

  // validate the incoming arguments now
  auto actual = args.at(0), expected = args.at(1);
  if (actual != expected) return Value::Void();

  // get the incoming message to be shown now
  auto message = args.at<Value::Any>(2, Value::Void());

  // otherwise we resolve a suitable error message
  return message.is<String::Any>() ? m_panic(isolate, 7000000, message) : m_panic(isolate, 7000004);
}

SABRE_MM_DYLIB_METHOD(Assert, panics, isolate, args) {
  // ensure we immediately fail if there are no arguments
  if (args.empty()) return isolate->panic(7000002, "exists");

  // ensure the leading argument is a function
  SABRE_MM_ASSERT_TYPEOF(isolate, Function::Any, args[0]);

  // get the incoming function to be called now
  auto result = isolate->spawn(args.at(0));

  // if the result is an error, then stop
  if (!result.has_value()) return Value::Void();

  // get the incoming message to be shown now
  auto message = args.at<Value::Any>(1, Value::Void());

  // otherwise we resolve a suitable error message for when no panic occurs
  return message.is<String::Any>() ? m_panic(isolate, 7000000, message) : m_panic(isolate, 7000005);
}

//  PRIVATE METHODS  //

Sabre::Value::Any Sabre::Package::Assert::m_test(
    Runtime::Isolate *isolate, const Function::Args &args, bool state, const $::String::View &method
) {
  // fail if there are no arguments given at all
  if (args.empty()) return isolate->panic(7000002, method);

  // ensure that the truthiness is valid now
  if (args.at(0).truthiness() == state) return Value::Void();

  // get the incoming message to be shown now
  auto message = args.at<Value::Any>(1, Value::Void());

  // otherwise we resolve a suitable error message
  if (message.is<String::Any>()) return m_panic(isolate, 7000000, message);
  return m_panic(isolate, 7000001, state ? "falsey" : "truthy"); // reason
}

Sabre::Value::Any Sabre::Package::Assert::m_panic(Runtime::Isolate *isolate, const Object::Exception &exception) {
  auto *services = isolate->service<XI::Container>();
  auto *service = services->when<Testing::Service>();
  if (service == nullptr) return isolate->panic(exception);

  // prepare the baseline reason and details
  auto because = exception.message().view();
  auto location = Testing::Inspect::location(exception);

  // prepare the reporter to be used now
  service->session()->asserts()->fail(because, location);

  // and emit that no error occured here
  return Value::Void();
}
