#ifndef _XTDLIB_ENCODING_UTF8_HPP
#define _XTDLIB_ENCODING_UTF8_HPP

/// Library Includes
#include "xtdlib/macros/attributes.hpp"
#include "xtdlib/string/buffer.hpp"
#include "xtdlib/string/view.hpp"

namespace $::Encoding::UTF8 {

//  PUBLIC METHODS  //

/**
 * @brief Determines the length of a code-point.
 * @param cp                    UTF-8 to check.
 */
inline constexpr uint32_t length(uint32_t cp) {
  if (cp < 0x80) return 1;
  else if (cp < 0x800) return 2;
  else if (cp < 0x10000) return 3;
  else return 4; // default max
}

/**
 * @brief Checks a buffers sequence length.
 * @param buffer                Buffer to check.
 */
inline constexpr uint32_t length(const char *buffer) {
  auto lead = static_cast<uint8_t>(*buffer);
  if (lead < 0x80) return 1;
  else if ((lead >> 5) == 0x06) return 2;
  else if ((lead >> 4) == 0x0e) return 3;
  else if ((lead >> 3) == 0x1e) return 4;
  return 1; // always default to one here
}

/**
 * @brief Checks how long a UTF-8 string is.
 * @param view                  String to check.
 */
size_t count(const char *buffer, size_t size);

/**
 * @brief Checks how long a UTF-8 string is.
 * @param view                  String to check.
 */
size_t count(const $::String::View &view);

/**
 * @brief Converts a code-point to a string.
 * @param cp                    Code-point to convert.
 */
$::String::Buffer from(uint32_t cp);

/**
 * @brief Gets a code-point from a string.
 * @param view                  View to resolve.
 * @param unit                  Codepoint index.
 */
uint32_t rune(const $::String::View &view, size_t unit);
uint32_t rune(const char *buffer, size_t bytes, size_t unit);
uint32_t rune(const char *buffer, const char *end, size_t unit);

/**
 * @brief Gets the equivalent UTF-8 codepoints.
 * @param view                  View to resolve.
 */
std::vector<uint32_t> units(const $::String::View &view);
std::vector<uint32_t> units(const char *buffer, size_t bytes);

/**
 * @brief Gets an underlying offset.
 * @param view                  View to resolve.
 * @param unit                  Codepoint index.
 */
size_t offset(const $::String::View &view, size_t unit);
size_t offset(const char *buffer, size_t bytes, size_t unit);
size_t offset(const char *buffer, const char *end, size_t unit);
size_t offset(const std::span<const uint32_t> &span, size_t unit);

/**
 * @brief Validates a given code-point.
 * @param cp                    UTF-8 to check.
 * @param length                Expected byte-length.
 */
inline constexpr bool validate(uint32_t cp, uint32_t length = 0) {
  auto valid = cp <= 0x0010ffff && !(cp >= 0x0000d800 && cp <= 0x0000dfff);
  if (!length) return valid; // should return immediately if not checking length

  if (cp < 0x80) return valid && length == 1;
  else if (cp < 0x800) return valid && length == 2;
  else if (cp < 0x10000) return valid && length == 3;
  else return valid && length == 4;
}

/**
 * @brief Peeks a codepoint value.
 * @param buffer                UTF-8 buffer.
 * @param end                   End of buffer.
 * @param cp                    Code-point output.
 */
$_NODISCARD inline constexpr bool peek(const char *&buffer, const char *end, uint32_t &cp) {
  if (buffer >= end) return false; // declare as failed now
  const char *original = buffer;   // clone the buffer instance
  auto len = length(buffer);       // get the sequence length now

  // prepare an incrementer for the buffer
  auto increment_safely = [&] {
    if (++buffer == end) return false;
    auto trail = static_cast<uint16_t>(*buffer);
    return ((trail & 0xff) >> 6) == 0x2;
  };

  // attempt decoding the necessary code-point now
  switch (len) {
  case 0: $_FALLTHROUGH;
  default: return false;

  case 1: {
    cp = static_cast<uint8_t>(*buffer);
    return ++buffer, validate(cp, len); // ensure valid
  }

  case 2: {
    cp = static_cast<uint8_t>(*buffer);
    if (!increment_safely()) break;
    cp = ((cp << 6) & 0x7FF) + ((*buffer) & 0x3F);
    return ++buffer, validate(cp, len); // ensure valid now
  }

  case 3: {
    cp = static_cast<uint8_t>(*buffer);
    if (!increment_safely()) break;
    cp = ((cp << 12) & 0xFFFF) + ((*buffer << 6) & 0xFFF);
    if (!increment_safely()) break;
    cp += (*buffer) & 0x3F;
    return ++buffer, validate(cp, len);
  }

  case 4: {
    cp = static_cast<uint8_t>(*buffer);
    if (!increment_safely()) break;
    cp = ((cp << 18) & 0x1FFFFF) + ((*buffer << 12) & 0x3FFFF);
    if (!increment_safely()) break;
    cp += (*buffer << 6) & 0xFFF;
    if (!increment_safely()) break;
    cp += (*buffer) & 0x3F;
    return ++buffer, validate(cp, len);
  }
  }

  // if we reach here, then reset the buffer
  return buffer = original, false;
}

/**
 * @brief Pushes the buffer one code-point forward.
 * @param buffer                Buffer to push.
 * @param end                   End of buffer.
 */
$_NODISCARD inline constexpr bool next(const char *&buffer, const char *end) {
  // prepare a no-operation code-point
  uint32_t cp;

  // and return the final result now
  return peek(buffer, end, cp);
}

} // namespace $::Encoding::UTF8

#endif
