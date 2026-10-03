#ifndef _XTDLIB_FILESYSTEM_READ_HPP
#define _XTDLIB_FILESYSTEM_READ_HPP

/// C++ Includes
#include <vector>

/// Library Includes
#include "xtdlib/debug/assert.hpp"
#include "xtdlib/filesystem/path.hpp"
#include "xtdlib/pointer/shared.hpp"
#include "xtdlib/string/buffer.hpp"
#include "xtdlib/string/view.hpp"

namespace $::FS {

/// @brief Internalized region view.
class Region {
  //  TYPEDEFS  //

  /// @brief Internal implemenation.
  class Wrapper;

  /// @brief Ensure the reader is allowed access.
  friend Region Read(const String::View &);

  //  PROPERTIES  //

  /// @brief The internal region instance.
  Shared::Pointer<Wrapper> m_internal = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Do not allow default construction.
  explicit Region();

  /**
   * @brief Constructs bounded buffer.
   * @param buffer
   */
  explicit Region(const std::span<const uint8_t> &buffer);

  /**
   * @brief Constructs bounded content.
   * @param content                 Content to bind.
   */
  explicit Region(const $::String::View &content);

  /**
   * @brief Constructs the internal region.
   * @param internal                Implementation to bind.
   */
  explicit Region(Shared::Pointer<Wrapper> &&internal);

  //  PUBLIC METHODS  //

  /// @brief Denotes if the region is empty.
  bool empty() const noexcept;

  /// @brief Gets the size of the region.
  size_t size() const noexcept;

  /// @brief Gets the data representation of the region.
  void *data() const noexcept;

  /// @brief Gets a basic string-view of the region.
  String::View view() const noexcept;

  /// @brief Gets a buffer span of the region.
  std::span<uint8_t> span() const noexcept;
};

/**
 * @brief Reads a file into memory.
 * @param file_path             File to read.
 */
Region Read(const Path &file_path);
Region Read(const String::View &file_path);
Region Read(const String::Buffer &file_path);

} // namespace $::FS

#endif
