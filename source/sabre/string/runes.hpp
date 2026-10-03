#ifndef _SABRE_STRING_RUNES_HPP
#define _SABRE_STRING_RUNES_HPP

/// Sabre Includes
#include "sabre/value/symbol.hpp"

namespace Sabre::String {

/// @brief Contains a string as code-points.
class Runes {
  //  PROPERTIES  //

  /// @brief Total runes available.
  size_t m_size = 0;

  /// @brief Total size in bytes.
  size_t m_bytes = 0;

  /// @brief All encoded string-buffer.
  $::Shared::Pointer<char[]> m_data = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs an empty set of runes.
  constexpr Runes() = default;

  /**
   * @brief Constructs a set of runes.
   * @param data              Incoming data.
   * @param bytes             Size in bytes.
   */
  constexpr Runes(const char *data) : Runes(data, std::strlen(data)) {}
  constexpr Runes(const char *data, size_t bytes) : m_bytes(bytes), m_data(m_decode(data, bytes)) {}

  /**
   * @brief Constructs a set of runes.
   * @param buffer            Buffer to encapsulate.
   */
  constexpr Runes(const $::String::View &buffer) : Runes(buffer.data(), buffer.size()) {}

  //  PUBLIC METHODS  //

  inline constexpr size_t size() const noexcept { return m_size; }
  inline constexpr size_t bytes() const noexcept { return m_bytes; }
  inline constexpr bool empty() const noexcept { return m_bytes == 0; }
  inline constexpr bool ascii() const noexcept { return m_size == m_bytes; }

  inline constexpr const char *data() const noexcept { return m_data.get(); }
  inline constexpr $::String::View view() const noexcept { return {m_data.get(), m_bytes}; }
  inline constexpr Value::Symbol symbol() const noexcept { return Value::Symbol(view()); }

  /**
   * @brief Gets the offset from a given index.
   * @param unit                  Unit index to resolve.
   */
  inline constexpr size_t offset(size_t unit) const { return m_offset(unit); }

  /**
   * @brief Reads a codepoint rune from the string.
   * @param unit                  Unit index expected.
   */
  inline constexpr uint32_t codepoint(size_t unit) const { return m_codepoint(unit); }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles copying across data.
   * @param data              Incoming data to copy.
   */
  inline constexpr $::Shared::Pointer<char[]> m_copy(const char *data, size_t bytes) const noexcept {
    if ($_UNLIKELY(bytes == 0)) return nullptr;
    auto ptr = std::make_shared<char[]>(bytes);
    return std::memcpy(ptr.get(), data, bytes), std::move(ptr);
  }

  /**
   * @brief Converts a unit index to an offset.
   * @param unit              Unit to convert.
   */
  size_t m_offset(size_t unit) const noexcept;

  /**
   * @brief Reads a codepoint from the set of runes.
   * @param unit              Unit to read.
   */
  uint32_t m_codepoint(size_t unit) const noexcept;

  /**
   * @brief Handles decoding data to be contained.
   * @param data              Incoming data to copy.
   */
  $::Shared::Pointer<char[]> m_decode(const char *data, size_t bytes) noexcept;

  /**
   * @brief Handles printing runes.
   * @param os                    Output stream.
   * @param self                  Runes instance.
   */
  static inline void m_print(std::ostream &os, const Runes &self) { os << self.view(); }
};

} // namespace Sabre::String

#endif
