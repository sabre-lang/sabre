/// Sabre Includes
#include "sabre/codec/toml.hpp"

/// Crate Includes
#include "crates/toml/source/addon.hpp"

//  PROPERTIES  //

/// @brief The underlying TOML addon installer.
SABRE_MM_DYLIB_ADDON(TOML, CRATE_XX_TOML_METHODS)

//  ADDON METHODS  //

SABRE_MM_DYLIB_METHOD(TOML, encode, isolate, args) {
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  return Sabre::Codec::TOML::encode(isolate, args.at(0));
}

SABRE_MM_DYLIB_METHOD(TOML, decode, isolate, args) {
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);
  return Sabre::Codec::TOML::encode(isolate, args.at<String::Any>(0));
}
