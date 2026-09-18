#ifndef _SABRE_ARCHIVE_SLICE_HPP
#define _SABRE_ARCHIVE_SLICE_HPP

/// Sabre Includes
#include "sabre/forward/image.hpp"
#include "sabre/value/pointer.hpp"

namespace Sabre::Image {

/// @brief Image Address Typing.
using Address = Pointer::Underlying;

/// @brief Image Bytes Buffer.
using Buffer = std::vector<uint8_t>;

/// @brief Sliced Code View.
class Slice {
  //  PROPERTIES  //

  /// @brief Gets the underlying size.
  size_t m_size = 0;

  /// @brief Encapsulated address value.
  size_t m_offset = 0;

  /// @brief The associated buffer value.
  const Buffer *m_buffer = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a buffer slice.
   * @param offset                    Offset to bind.
   * @param size                      Size of the view.
   * @param buffer                    Internal buffer value.
   */
  constexpr Slice() = default;
  constexpr Slice(size_t offset, size_t size, const Buffer *buffer) :
      m_size(size), m_offset(offset), m_buffer(buffer) {}

  //  PUBLIC METHODS  //

  /// @brief Handles validating the underlying address.
  inline constexpr bool valid() const noexcept { return m_buffer != nullptr; }

  /// @brief The size of the view.
  inline constexpr size_t size() const noexcept { return m_size; }

  /// @brief The offset from the start of the buffer.
  inline constexpr size_t offset() const noexcept { return m_offset; }

  /// @brief Gets the underlying address value.
  inline constexpr Address address() const noexcept {
    return m_buffer ? reinterpret_cast<Address>(m_buffer->data()) + m_offset : 0;
  }
};

} // namespace Sabre::Image

#endif
