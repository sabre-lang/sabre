#ifndef _SABRE_STRING_RUNES_HPP
#define _SABRE_STRING_RUNES_HPP

/// Sabre Includes
#include "sabre/value/symbol.hpp"

namespace Sabre::String {

/// @brief Contains a string as code-points.
class Runes {
  //  TYPEDEFS  /

  /// @brief The underlying encoding offsets.
  enum class Offset : size_t {
    BYTE_SIZE,
    UTF8_SIZE,
    UTF8_DATA,
  };

  //  PROPERTIES  //

  /// @brief All encoded string-buffer.
  void *m_data = nullptr;

  /// @brief The decoded lengths available.
  void *m_units = nullptr;

  // prepare our offset sizing to be used
  static inline constexpr size_t m_offset = sizeof(Offset);

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
  constexpr Runes(const char *data, size_t bytes) {
    // ignore if there are no bytes to resolve
    if (bytes == 0) return;

    m_data = m_copy(data, bytes);    // copy the data
    m_units = m_decode(data, bytes); // and decode units
  }

  /**
   * @brief Constructs a set of runes.
   * @param buffer            Buffer to encapsulate.
   */
  constexpr Runes(const $::String::View &buffer) : Runes(buffer.data(), buffer.size()) {}

  /**
   * @brief The copy construct just inherits the base constructor.
   * @param other             Other runes buffer.
   */
  constexpr Runes(const Runes &other) : Runes(other.data(), other.bytes()) {}

  /**
   * @brief The move constructor simply moves data.
   * @param other             Other item to swap.
   */
  constexpr Runes(Runes &&other) : m_data(other.m_data), m_units(other.m_units) {
    other.m_data = other.m_units = nullptr;
  }

  /// @brief Handles deallocating runes.
  constexpr ~Runes() { m_destruct(); }

  //  OPERATOR METHODS  //

  /// @brief Handles copying across data from another set of runes.
  inline constexpr Runes &operator=(const Runes &other) noexcept {
    // remove any underlying data if available
    m_destruct();

    // and we copy across any incoming data now
    m_data = other.m_data ? m_copy(other.m_data, other.bytes()) : nullptr;
    m_units = other.m_units ? m_copy(other.m_units, (2 + other.size()) * m_offset) : nullptr;

    // return the resulting reference now
    return *this;
  }

  /// @brief Handles moving other items.
  inline constexpr Runes &operator=(Runes &&other) noexcept {
    // ensure the properties are moved
    m_data = other.m_data;
    m_units = other.m_units;

    // and then clear the other item
    other.m_data = nullptr;
    other.m_units = nullptr;

    // and return the resulting details now
    return *this;
  }

  //  PUBLIC METHODS  //

  inline constexpr bool empty() const noexcept { return m_data == nullptr; }
  inline constexpr size_t size() const noexcept { return m_size(Offset::UTF8_SIZE); }
  inline constexpr size_t bytes() const noexcept { return m_size(Offset::BYTE_SIZE); }
  inline constexpr Value::Symbol symbol() const noexcept { return Value::Symbol(view()); }
  inline constexpr const char *data() const noexcept { return static_cast<const char *>(m_data); }

  inline constexpr $::String::View view() const noexcept { return {data(), bytes()}; }
  inline constexpr std::span<const size_t> units() const noexcept { return {m_at(Offset::UTF8_DATA), size()}; }

protected:
  //  PRIVATE METHODS  //

  /// @brief Handles removing internal data.
  inline constexpr void m_destruct() {
    if (m_data) std::free(m_data), m_data = nullptr;
    if (m_units) std::free(m_units), m_units = nullptr;
  }

  /**
   * @brief Handles copying across data.
   * @param data              Incoming data to copy.
   */
  template <class T> inline constexpr void *m_copy(const T *data, size_t bytes) const noexcept {
    return std::memcpy(std::malloc(bytes), static_cast<const void *>(data), bytes);
  }

  /**
   * @brief Decode the incoming data code-points.
   * @param data                  Buffer data.
   * @param bytes                 Size in bytes.
   */
  inline constexpr void *m_decode(const char *data, size_t bytes) const noexcept {
    // prepare the outgoing units to be used
    auto units = std::vector<size_t>();

    // attempt reading the incoming lengths now
    for (size_t ii = 0; ii < bytes;) units.emplace_back(ii), ii += $::Encoding::UTF8::length(data + ii);

    // return the decoded result now
    auto output = static_cast<size_t *>(std::malloc((2 + units.size()) * m_offset));

    *output = bytes, *(output + 1) = units.size(); // copy across
    std::memcpy(output + 2, units.data(), units.size() * m_offset);

    // finally return the output now
    return output;
  }

  /**
   * @brief Gets the byte-offset from sizes.
   * @param offset                Size offset.
   */
  inline constexpr const size_t *m_at(Offset offset) const noexcept {
    return static_cast<const size_t *>(m_units) + static_cast<size_t>(offset);
  }

  /**
   * @brief Constructs a size value.
   * @param offset                Size offset.
   */
  inline constexpr size_t m_size(Offset offset) const noexcept { return m_units ? *m_at(offset) : 0; }

  /**
   * @brief Handles printing runes.
   * @param os                    Output stream.
   * @param self                  Runes instance.
   */
  static inline void m_print(std::ostream &os, const Runes &self) { os << self.view(); }
};

} // namespace Sabre::String

#endif
