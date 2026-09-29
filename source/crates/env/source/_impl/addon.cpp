/// Crate Includes
#include "crates/env/source/addon.hpp"
#include "sabre/dotenv/service.hpp"

//  PROPERTIES  //

/// @brief The underlying Environ addon installer.
SABRE_MM_DYLIB_ADDON(Environ, CRATE_XX_ENV_METHODS)

//  ADDON METHODS  //

SABRE_MM_DYLIB_METHOD(Environ, get, isolate, args) {
  // ensure we are a desired number of arguments
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);

  // start pulling out the incoming arguments now
  auto key = args.at<String::Any>(0);

  // prepare the services to be used for this method
  auto *dotenv = isolate->service<Dotenv::Service>();

  // attempt resolution as necessary now
  auto value = dotenv->get(key.view());

  // if the value exists, then we return immediately
  if (value.has_value()) return String::Any(isolate, *value);

  // otherwise we need to instead resolve the alternative
  auto alt = args.at(1, String::Any());
  return alt.is<String::Any>() ? alt : String::Any();
}

SABRE_MM_DYLIB_METHOD(Environ, set, isolate, args) {
  // ensure we are a desired number of arguments
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 2);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[1]);

  // start pulling out the incoming arguments now
  auto key = args.at<String::Any>(0);
  auto value = args.at<String::Any>(1);

  // prepare the services to be used for this method
  auto *dotenv = isolate->service<Dotenv::Service>();

  // and assign as necessary
  return Value::Boolean(dotenv->set(key.view(), value.view()));
}

SABRE_MM_DYLIB_METHOD(Environ, drop, isolate, args) {
  // ensure we are a desired number of arguments
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);

  // start pulling out the incoming arguments now
  auto key = args.at<String::Any>(0);

  // prepare the services to be used for this method
  auto *dotenv = isolate->service<Dotenv::Service>();

  // and assign as necessary
  return Value::Boolean(dotenv->del(key.view()));
}
