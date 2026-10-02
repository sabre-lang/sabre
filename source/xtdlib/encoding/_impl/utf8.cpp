/// Vendor Includes
#include <simdutf.h>

/// Library Includes
#include "xtdlib/debug/assert.hpp"
#include "xtdlib/encoding/utf8.hpp"
#include "xtdlib/macros/forward.hpp"

/// Forward Declarations
$_FWD($::Encoding::UTF8, char *append(uint32_t, char *))

//  PUBLIC METHODS  //

/**
 * @brief Helper method to append to a buffer.
 * @param cp                    Unicode codepoint.
 * @param buffer                Buffer to append.
 */
inline char *$::Encoding::UTF8::append(uint32_t cp, char *buffer) {
  // declare as currently invalid now
  if (!$::Encoding::UTF8::validate(cp)) return nullptr;

  // emplace onto the buffer as needed
  if (cp < 0x80) *(buffer++) = static_cast<char>(cp);
  else if (cp < 0x800) {
    *(buffer++) = static_cast<char>((cp >> 6) | 0xc0);
    *(buffer++) = static_cast<char>((cp & 0x3f) | 0x80);
  } else if (cp < 0x10000) {
    *(buffer++) = static_cast<char>((cp >> 12) | 0xe0);
    *(buffer++) = static_cast<char>(((cp >> 6) & 0x3f) | 0x80);
    *(buffer++) = static_cast<char>((cp & 0x3f) | 0x80);
  } else {
    *(buffer++) = static_cast<char>((cp >> 18) | 0xf0);
    *(buffer++) = static_cast<char>(((cp >> 12) & 0x3f) | 0x80);
    *(buffer++) = static_cast<char>(((cp >> 6) & 0x3f) | 0x80);
    *(buffer++) = static_cast<char>((cp & 0x3f) | 0x80);
  }

  // return the final buffer value
  return buffer;
}

size_t $::Encoding::UTF8::count(const String::View &view) { return count(view.data(), view.size()); }
size_t $::Encoding::UTF8::count(const char *buffer, size_t size) { return simdutf::count_utf8(buffer, size); }

$::String::Buffer $::Encoding::UTF8::from(uint32_t cp) {
  char buffer[4]; // only expect maximum 4-bytes
  char *end = append(cp, buffer);
  return {buffer, static_cast<size_t>(end - buffer)};
}

uint32_t $::Encoding::UTF8::rune(const String::View &view, size_t unit) { return rune(view.data(), view.size(), unit); }

uint32_t $::Encoding::UTF8::rune(const char *buffer, size_t size, size_t unit) {
  return rune(buffer, buffer + size, unit);
}

uint32_t $::Encoding::UTF8::rune(const char *buffer, const char *end, size_t unit) {
  // prepare the code-point to be used now
  uint32_t cp;

  // attempt peeking until the necessary unit now
  for (size_t ii = 0; ii < unit; ++ii) $_ASSERT(buffer < end), buffer += length(buffer);

  // attempt running our necessary peek handler now
  return $_EXPECT(peek(buffer, end, cp), "Failed to read UTF-8 rune at {0}", unit), cp;
}

std::vector<uint32_t> $::Encoding::UTF8::units(const String::View &view) { return units(view.data(), view.size()); }
std::vector<uint32_t> $::Encoding::UTF8::units(const char *buffer, size_t bytes) {
  // determine some baseline details
  size_t written = 0;
  auto total = count(buffer, bytes);
  auto units = std::vector<uint32_t>(total);
  auto *output = reinterpret_cast<char32_t *>(units.data());

  // attempt iteratively converting with replacements
  for (size_t read = 0; read < bytes; ++written) {
    // prepare the leading details to be used
    const char *head = buffer + read;
    size_t remaining = bytes - read;

    auto state = simdutf::convert_utf8_to_utf32_with_errors(head, remaining, output);
    written += state.count; // update the current written count now as necessary

    // stop early when we have no errors occuring
    if (state.error == simdutf::error_code::SUCCESS) break;

    // otherwise we got an invalid sequence, so let's replace it now
    read += state.count + 1, *output++ = 0xFFFD;
  }

  // finally resize and return
  return units.resize(written), units;
}

size_t $::Encoding::UTF8::offset(const std::span<const uint32_t> &span, size_t unit) {
  return simdutf::utf8_length_from_utf32(reinterpret_cast<const char32_t *>(span.data()), unit);
}

size_t $::Encoding::UTF8::offset(const String::View &view, size_t unit) {
  return offset(view.data(), view.size(), unit);
}

size_t $::Encoding::UTF8::offset(const char *buffer, size_t size, size_t unit) {
  return offset(buffer, buffer + size, unit);
}

size_t $::Encoding::UTF8::offset(const char *buffer, $_UNUSED const char *end, size_t unit) {
  // prepare a resulting position now
  auto head = buffer;

  // attempt peeking until the necessary unit now
  for (size_t ii = 0; ii < unit; ++ii) $_ASSERT(buffer < end), buffer += length(buffer);

  // attempt running our necessary peek handler now
  return static_cast<size_t>(buffer - head);
}
