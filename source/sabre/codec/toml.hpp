#ifndef _SABRE_CODEC_TOML_HPP
#define _SABRE_CODEC_TOML_HPP

/// Sabre Includes
#include "sabre/string/common.hpp"

namespace Sabre::Codec::TOML {

/**
 * @brief Handles encoding TOML inputs.
 * @param isolate               Runtime isolate.
 * @param input                 Value to encode.
 */
Value::Any encode(Runtime::Isolate *isolate, const Value::Any &input);

/**
 * @brief Handles decoding TOML inputs.
 * @param isolate               Runtime isolate.
 * @param input                 String to decode.
 */
Value::Any decode(Runtime::Isolate *isolate, const String::Any &input);
Value::Any decode(Runtime::Isolate *isolate, const $::String::View &input);

} // namespace Sabre::Codec::TOML

#endif
