#ifndef _SABRE_STRING_RUNES_HPP
#define _SABRE_STRING_RUNES_HPP

/// Sabre Includes
#include "sabre/value/symbol.hpp"

namespace Sabre::String {

/// @brief Contains a string as code-points.
class Runes {
  //  PROPERTIES  //

  /// @brief Total rune bytes.
  size_t m_bytes = 0;

  /// @brief All encoded string-buffer.
  char *m_data = nullptr;

  /// @brief The flattened 32-bit values.
  std::vector<uint32_t> m_units = {};

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
  constexpr Runes(const char *data, size_t bytes) : m_bytes(bytes) {
    // ignore if there are no bytes to resolve
    if (bytes == 0) return;

    m_data = m_copy(data, bytes); // copy the data
    m_units = $::Encoding::UTF8::units(data, bytes);
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
  constexpr Runes(Runes &&other) : m_bytes(other.m_bytes), m_data(other.m_data), m_units(std::move(other.m_units)) {
    other.m_bytes = 0;
    other.m_data = nullptr;
  }

  /// @brief Handles deallocating runes.
  constexpr ~Runes() { m_destruct(); }

  //  OPERATOR METHODS  //

  /// @brief Handles copying across data from another set of runes.
  inline constexpr Runes &operator=(const Runes &other) noexcept {
    // we copy across any incoming data now as necessary
    m_units = other.m_units, m_bytes = other.m_bytes;
    m_data = other.m_data ? m_copy(other.m_data, other.m_bytes) : nullptr;

    // return the resulting reference now
    return *this;
  }

  /// @brief Handles moving other items.
  inline constexpr Runes &operator=(Runes &&other) noexcept {
    // ensure the properties are moved
    m_data = std::move(other.m_data);
    m_bytes = std::move(other.m_bytes);
    m_units = std::move(other.m_units);

    // and return the resulting details now
    return *this;
  }

  //  PUBLIC METHODS  //

  inline constexpr bool empty() const noexcept { return m_bytes == 0; }
  inline constexpr size_t bytes() const noexcept { return m_bytes; }
  inline constexpr size_t size() const noexcept { return m_units.size(); }
  inline constexpr Value::Symbol symbol() const noexcept { return Value::Symbol(view()); }
  inline constexpr const char *data() const noexcept { return m_data; }

  inline constexpr $::String::View view() const noexcept { return {m_data, bytes()}; }
  inline constexpr std::span<const uint32_t> units() const noexcept { return m_units; }

protected:
  //  PRIVATE METHODS  //

  /// @brief Handles removing internal data.
  inline constexpr void m_destruct() {
    if (m_data) std::free(static_cast<void *>(m_data)), m_data = nullptr;
  }

  /**
   * @brief Handles copying across data.
   * @param data              Incoming data to copy.
   */
  inline constexpr char *m_copy(const char *data, size_t bytes) const noexcept {
    return static_cast<char *>(std::memcpy(std::malloc(bytes), static_cast<const void *>(data), bytes));
  }

  /**
   * @brief Handles printing runes.
   * @param os                    Output stream.
   * @param self                  Runes instance.
   */
  static inline void m_print(std::ostream &os, const Runes &self) { os << self.view(); }
};

} // namespace Sabre::String

#endif
