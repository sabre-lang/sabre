/// Sabre Includes
#include "sabre/codec/json.hpp"

/// Value Includes
#include "sabre/value/_inline/value.ipp"

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Codec::JSON::encode(Runtime::Isolate *isolate, const Value::Any &) { return isolate->todo(); }

Sabre::Value::Any Sabre::Codec::JSON::decode(Runtime::Isolate *isolate, const String::Any &input) {
  return decode(isolate, input.view());
}

Sabre::Value::Any Sabre::Codec::JSON::decode(Runtime::Isolate *isolate, const $::String::View &) {
  return isolate->todo();
}
