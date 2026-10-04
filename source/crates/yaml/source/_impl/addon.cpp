/// Sabre Includes
#include "sabre/codec/yaml.hpp"

/// Crate Includes
#include "crates/yaml/source/addon.hpp"

//  PROPERTIES  //

/// @brief The underlying YAML addon installer.
SABRE_MM_DYLIB_ADDON(YAML, CRATE_XX_YAML_METHODS)

//  ADDON METHODS  //

SABRE_MM_DYLIB_METHOD(YAML, encode, isolate, args) {
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  return Sabre::Codec::YAML::encode(isolate, args.at(0));
}

SABRE_MM_DYLIB_METHOD(YAML, decode, isolate, args) {
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);
  return Sabre::Codec::YAML::encode(isolate, args.at<String::Any>(0));
}
