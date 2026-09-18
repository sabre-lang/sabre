#ifndef _SABRE_DOCUMENT_BUFFER_HPP
#define _SABRE_DOCUMENT_BUFFER_HPP

/// Sabre Includes
#include "sabre/forward/document.hpp"

namespace Sabre::Document {

/// @brief Document Buffer Abstraction.
class Buffer {
  //  PROPERTIES  //

  /// @brief Underlying memory buffer.
  $::String::Buffer m_buffer = "";

  /// @brief Reference resource value.
  $::URI::Buffer m_resource = $::URI::View().buffer();

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs an empty buffer value.
  constexpr Buffer() = default;

  /**
   * @brief Constructs a buffer.
   * @param file_path             File resource.
   */
  constexpr Buffer(const $::FS::Path &file_path) : m_resource($::URI::File(file_path)) {
    m_buffer = $::FS::Read(file_path);
  }

  /**
   * @brief Constructs a buffer.
   * @param resource              Document resource.
   */
  constexpr Buffer(const $::URI::View &resource) : m_resource(resource.buffer()) {
    $_ASSERT(resource.scheme() == $::URI::Scheme::FILE);
    m_buffer = $::FS::Read(resource.body()); // ensure
  }

  /**
   * @brief Constructs a buffer.
   * @param content               Content to bind.
   */
  constexpr Buffer(const $::String::View &content, const $::URI::View &resource = {})
      : m_buffer(content), m_resource(resource.buffer()) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the size of the buffer.
  inline constexpr size_t size() const { return m_buffer.size(); }

  /// @brief Gets the coinciding buffer data.
  inline constexpr const char *data() const { return m_buffer.data(); }

  /// @brief Gets the associated string-view.
  inline constexpr $::String::View view() const { return m_buffer; }

  /// @brief Potential resource available.
  inline constexpr $::URI::View resource() const noexcept { return m_resource; }

  /// @brief Checks if a buffer is anonymous at all.
  inline constexpr bool anonymous() const noexcept { return resource().anonymous(); }

  /// @brief Converts the buffer to a binary vector.
  inline constexpr std::vector<uint8_t> binary() const { return $::Ranges::To<uint8_t>(m_buffer); }
};

} // namespace Sabre::Document

#endif
