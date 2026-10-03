#ifndef _XTDLIB_MEMORY_REGION_HPP
#define _XTDLIB_MEMORY_REGION_HPP

/// Library Includes
#include "xtdlib/filesystem/read.hpp"
#include "xtdlib/pointer/shared.hpp"
#include "xtdlib/string/view.hpp"

namespace $::Memory {

/// @brief Internalized region view.
class Region {
  //  TYPEDEFS  //

  /// @brief Internal implemenation.
  class Wrapper;

  /// @brief Allow the filesystem internal access.
  friend Region FS::Read(const String::View &);

  //  PROPERTIES  //

  /// @brief The internal region instance.
  Shared::Pointer<Wrapper> m_internal = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Do not allow default construction.
  Region();

  /**
   * @brief Fills a region with a desired capacity.
   * @param capacity                Capacity to give.
   */
  explicit Region(size_t capacity);

  /**
   * @brief Constructs bounded content.
   * @param content                 Content to bind.
   */
  explicit Region(const String::View &content);

  /**
   * @brief Constructs bounded buffer.
   * @param buffer                  Buffer to bind.
   */
  explicit Region(const std::span<const uint8_t> &buffer);

  /**
   * @brief Constructs a region from raw values.
   * @param data                    Data to bind.
   * @param size                    Size of data.
   */
  explicit Region(const void *data, size_t size);

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

} // namespace $::Memory

#endif
