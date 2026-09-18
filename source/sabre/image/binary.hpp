#ifndef _SABRE_ARCHIVE_BINARY_HPP
#define _SABRE_ARCHIVE_BINARY_HPP

/// Sabre Includes
#include "sabre/image/records.hpp"
#include "sabre/image/slice.hpp"

namespace Sabre::Image {

/// @brief Image Code Binary.
class Binary {
  //  PROPERTIES  //

  /// @brief Internal buffer value.
  Buffer m_buffer = {};

  /// @brief Available bytecode-lookup.
  Records m_records = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a code buffer.
   * @param capacity              Initial capacity.
   */
  constexpr Binary(size_t capacity = 1024) { m_buffer.reserve(capacity); }

  /**
   * @brief Constructs a code buffer.
   * @param buffer                Code to emplace.
   */
  constexpr Binary(const Buffer &buffer) : m_buffer(buffer) {}

  //  PUBLIC METHODS  //

  inline constexpr bool empty() const noexcept { return m_buffer.empty(); }
  inline constexpr size_t size() const noexcept { return m_buffer.size(); }

  inline constexpr const Buffer &buffer() const noexcept { return m_buffer; }
  inline constexpr const uint8_t *data() const noexcept { return m_buffer.data(); }
  inline constexpr Address address() const noexcept { return reinterpret_cast<Address>(m_buffer.data()); }

  /// @brief The internal source-records for sources.
  inline constexpr Records &records() noexcept { return m_records; }
  inline constexpr const Records &records() const noexcept { return m_records; }

  /**
   * @brief Gets a view of the buffer.
   * @param offset                Offset value.
   * @param size                  Size of view.
   */
  inline constexpr Slice slice(size_t offset = 0) const noexcept { return slice(offset, size() - offset); }
  inline constexpr Slice slice(size_t offset, size_t size) const noexcept { return Slice(offset, size, &m_buffer); }

  /**
   * @brief Writes a value to the buffer.
   * @param value                 Value to write.
   */
  template <std::unsigned_integral T> inline constexpr Binary &write(T value) {
    auto *data = std::bit_cast<const uint8_t *>(&value); // prepare data view
    for (size_t ii = 0; ii < sizeof(T); ++ii) m_buffer.emplace_back(data[ii]);
    return *this; // and allow chaining writes for easier use
  }

  /**
   * @brief Patches a value to the buffer.
   * @param offset                Offset to patch.
   * @param value                 Value to write.
   */
  template <std::unsigned_integral T> inline constexpr Binary &patch(size_t offset, T value) {
    auto *data = std::bit_cast<const uint8_t *>(&value); // prepare data view
    for (size_t ii = 0; ii < sizeof(T); ++ii) m_buffer[offset + ii] = data[ii];
    return *this; // and allow chaining writes for easier use
  }
};

} // namespace Sabre::Image

#endif
