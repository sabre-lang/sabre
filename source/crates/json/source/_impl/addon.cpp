/// Sabre Includes
#include "sabre/codec/json.hpp"

/// Crate Includes
#include "crates/json/source/addon.hpp"

//  PROPERTIES  //

/// @brief The underlying JSON addon installer.
SABRE_MM_DYLIB_ADDON(JSON, CRATE_XX_JSON_METHODS)

//  ADDON METHODS  //

SABRE_MM_DYLIB_METHOD(JSON, encode, isolate, args) {
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  return Sabre::Codec::JSON::encode(isolate, args.at(0));
}

SABRE_MM_DYLIB_METHOD(JSON, decode, isolate, args) {
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);
  return Sabre::Codec::JSON::encode(isolate, args.at<String::Any>(0));
}
