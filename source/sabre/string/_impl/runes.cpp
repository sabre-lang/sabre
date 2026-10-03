/// Vendor Includes
#include <simdutf.h>

/// Sabre Includes
#include "sabre/string/runes.hpp"

//  PRIVATE METHODS  //

size_t Sabre::String::Runes::m_offset(size_t unit) const noexcept {
  // for simple ASCII values we return immediately
  if (ascii() || unit == 0) return unit;

  // if the size is too large, then stop early
  if (unit >= m_size) return m_bytes;

  // prepare a minimum and maximum bound for offset to be used
  return $::Encoding::UTF8::offset(view(), unit);
}

uint32_t Sabre::String::Runes::m_codepoint(size_t unit) const noexcept {
  // if we have simple ASCII values, then immediately jump
  if (ascii() || unit == 0) return static_cast<uint32_t>(m_data[unit]);

  // prepare a suitable length of the incoming string view now
  auto total = simdutf::utf32_length_from_utf8(view());
  auto transcoded = std::vector<char32_t>(total);

  // get the total number of "utf-32" characters available
  $_UNUSED $_AUTO = simdutf::convert_valid_utf8_to_utf32(view(), transcoded);

  // return the nth codepoint as necesssary now
  return transcoded[unit];
}

$::Shared::Pointer<char[]> Sabre::String::Runes::m_decode(const char *data, size_t bytes) noexcept {
  // pre-check if we have some valid ASCII
  if (simdutf::validate_ascii(data, bytes)) return m_size = bytes, m_copy(data, bytes);

  // check for what type of encoding we are being given
  switch (simdutf::autodetect_encoding(data, bytes)) {
  // "utf-8" values should be normally encoded
  case simdutf::encoding_type::UTF8: {
    m_size = simdutf::count_utf8(data, bytes);
    return m_copy(data, bytes); // safely copy
  }

  // simple conversion from malformed values through "latin1" encoding
  case simdutf::encoding_type::unspecified: $_FALLTHROUGH;
  case simdutf::encoding_type::Latin1: {
    m_size = simdutf::utf8_length_from_latin1(data, bytes);
    auto buffer = std::vector<char>(m_size); // prepare buffer now
    m_size = simdutf::convert_latin1_to_utf8(data, bytes, buffer.data());
    return buffer.resize(m_size), m_copy(buffer.data(), m_bytes = buffer.size());
  }

  // for now declare that other conversions are invalid
  default: $_ABORT("Invalid text encoding");
  }
}
