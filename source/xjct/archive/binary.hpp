#ifndef _XJCT_ARCHIVE_BINARY_HPP
#define _XJCT_ARCHIVE_BINARY_HPP

/// XJCT Modules
#include "xjct/archive/format.hpp"

namespace XJCT::Archive {

/// @brief Executable Binary Container.
class Binary {
  //  PROPERTIES  //

  /// @brief Associated binary buffer.
  Blob::Bytes m_buffer = {};

  /// @brief Underlying archival format.
  Format m_format = Format::UNKNOWN;

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs an empty binary.
  explicit Binary() = default;

  /**
   * @brief Constructs a binary from a given buffer.
   * @param buffer                Buffer to bind.
   */
  explicit Binary(const $::FS::Region &buffer) : Binary($::Ranges::To(buffer.span())) {}

  /**
   * @brief Constructs a binary from a given buffer.
   * @param buffer                Buffer to bind.
   */
  explicit Binary(const Blob::Bytes &buffer) : m_buffer(buffer), m_format(m_deduce(m_buffer)) {}

  /**
   * @brief Constructs a binary from an executable path.
   * @param executable            Executable to read.
   */
  explicit Binary(const $::FS::Path &executable) : Binary($::FS::Read(executable)) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the format of the binary.
  inline constexpr Format format() const noexcept { return m_format; }

  /// @brief Gets the underlying buffer.
  inline constexpr Blob::Bytes &buffer() noexcept { return m_buffer; }
  inline constexpr const Blob::Bytes &buffer() const noexcept { return m_buffer; }

  /// @brief Gets the underlying buffer as a span.
  inline constexpr std::span<uint8_t> span() noexcept { return m_buffer; }
  inline constexpr std::span<const uint8_t> span() const noexcept { return m_buffer; }

  /// @brief Gets the binary as a blob.
  inline constexpr Blob::View blob() const noexcept {
    return {reinterpret_cast<const char *>(m_buffer.data()), m_buffer.size()};
  };

  /// @brief Gets the expected binary extension.
  inline constexpr $::String::View extension() const noexcept { return m_format == Format::WINDOWS ? ".exe" : ""; }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Resolves the format of a binary.
   * @param buffer                    Buffer to deduce.
   */
  static Format m_deduce(const Blob::Bytes &buffer) noexcept;
};

} // namespace XJCT::Archive

#endif
