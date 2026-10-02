#ifndef _SABRE_CODEC_JSON_HPP
#define _SABRE_CODEC_JSON_HPP

/// Sabre Includes
#include "sabre/string/common.hpp"

namespace Sabre::Codec::JSON {

/**
 * @brief Handles encoding JSON inputs.
 * @param isolate               Runtime isolate.
 * @param input                 Value to encode.
 */
Value::Any encode(Runtime::Isolate *isolate, const Value::Any &input);

/**
 * @brief Handles decoding JSON inputs.
 * @param isolate               Runtime isolate.
 * @param input                 String to decode.
 */
Value::Any decode(Runtime::Isolate *isolate, const String::Any &input);
Value::Any decode(Runtime::Isolate *isolate, const $::String::View &input);

} // namespace Sabre::Codec::JSON

#endif
