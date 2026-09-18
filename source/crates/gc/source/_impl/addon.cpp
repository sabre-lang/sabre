/// Crate Includes
#include "crates/gc/source/addon.hpp"

//  PROPERTIES  //

/// @brief The underlying garbage addon installer.
SABRE_MM_DYLIB_ADDON(Garbage, CRATE_XX_GARBAGE_METHODS)

//  ADDON METHODS  //

SABRE_MM_DYLIB_METHOD(Garbage, cycles, isolate, ) {
  Service *garbage = *isolate->service();
  return Number::Tagged(garbage->cycles());
}

SABRE_MM_DYLIB_METHOD(Garbage, collect, isolate, args) {
  auto value = args.at(0, Value::Boolean(0)); // prepare
  SABRE_MM_ASSERT_TYPEOF(isolate, Value::Boolean, value);
  auto major = value.as<Value::Boolean>().state(); // pull

  // prepare the collection service to be used
  Service *garbage = *isolate->service();

  // and request a collection to occur now
  isolate->thread()->native([major, garbage] { garbage->collect(major); });

  // ensure we resolve as done now
  return Value::Void();
}
