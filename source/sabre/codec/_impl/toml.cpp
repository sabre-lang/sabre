/// Sabre Includes
#include "sabre/codec/toml.hpp"
#include "sabre/runtime/isolate.hpp"

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Codec::TOML::encode(Runtime::Isolate *isolate, const Value::Any &) { return isolate->todo(); }
Sabre::Value::Any Sabre::Codec::TOML::decode(Runtime::Isolate *isolate, const String::Any &) { return isolate->todo(); }
Sabre::Value::Any Sabre::Codec::TOML::decode(Runtime::Isolate *isolate, const $::String::View &) {
  return isolate->todo();
}
