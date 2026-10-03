#ifndef _XJCT_ARCHIVE_BINARY_HPP
#define _XJCT_ARCHIVE_BINARY_HPP

/// LIEF Modules

/// XJCT Modules
#include "xjct/archive/format.hpp"

namespace XJCT::Archive {

/// @brief Executable Binary Container.
class Binary {
  //  PROPERTIES  //

  /// @brief Associated binary buffer.
  $::Memory::Region m_buffer = {};

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
  explicit Binary($::Memory::Region &&buffer) : m_buffer(std::move(buffer)), m_format(m_deduce(span())) {}

  /**
   * @brief Constructs a binary from an executable path.
   * @param executable            Executable to read.
   */
  explicit Binary(const $::FS::Path &executable) : Binary($::FS::Read(executable)) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the format of the binary.
  inline constexpr Format format() const noexcept { return m_format; }

  /// @brief Gets the size of the binary.
  inline constexpr size_t size() const noexcept { return m_buffer.size(); }

  /// @brief Gets the data for the binary.
  inline constexpr const uint8_t *data() const noexcept { return static_cast<const uint8_t *>(m_buffer.data()); }

  /// @brief Gets the underlying buffer as a span.
  inline constexpr std::span<uint8_t> span() noexcept { return m_buffer.span(); }
  inline constexpr std::span<const uint8_t> span() const noexcept { return m_buffer.span(); }

  /// @brief Allow updating the internal buffer.
  inline constexpr bool update($::Memory::Region &&buffer) noexcept { return m_buffer = std::move(buffer), true; }
  inline constexpr bool update(const std::vector<uint8_t> &buffer) noexcept {
    return update($::Memory::Region(buffer));
  }

  /// @brief Gets the binary as a blob.
  inline constexpr Blob::View blob() const noexcept { return m_buffer.view(); };

  /// @brief Gets the expected binary extension.
  inline constexpr $::String::View extension() const noexcept { return m_format == Format::WINDOWS ? ".exe" : ""; }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Resolves the format of a binary.
   * @param buffer                    Buffer to deduce.
   */
  static Format m_deduce(const std::span<const uint8_t> &buffer) noexcept;
};

} // namespace XJCT::Archive

#endif
