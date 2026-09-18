#ifndef _SABRE_HEAP_BUFFER_HPP
#define _SABRE_HEAP_BUFFER_HPP

/// Sabre Includes
#include "sabre/heap/region.hpp"

namespace Sabre::Heap {

/// @brief Helper for aligning region buffers.
template <size_t A = Pointer::Alignment::OBJECT> inline constexpr size_t Align(size_t size) {
  // get the current remainder value
  auto remainder = size % A;

  // attempt resolving a suitable alignment now
  return size + (remainder ? A - remainder : 0);
}

/// @brief Allocation Buffer.
class Buffer : public XI::Transient {
  //  PROPERTIES  //

  /// @brief The bound generation to allocate.
  Epoch m_epoch = Epoch::EDEN;

  /// @brief Heap manager.
  Service *m_heap = nullptr;

  /// @brief Acquired region.
  Region *m_region = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a heap allocation buffer.
   * @param heap                  Heap manager.
   */
  explicit Buffer(Epoch epoch = Epoch::EDEN);
  explicit Buffer(Service *heap, Epoch epoch = Epoch::EDEN);
  explicit Buffer(XI::Container *services, Epoch epoch = Epoch::EDEN);

  //  PUBLIC METHODS  //

  /// @brief Gets a view of the underlying region.
  inline constexpr const Region *region() const noexcept { return m_region; }

  /**
   * @brief Checks if an allocation will fit.
   * @param size                   Size to allocate.
   */
  inline constexpr bool fits(size_t size) const noexcept { return m_region->fits(size); }

  /**
   * @brief Handles allocating memory on this buffer.
   * @param isolate               Thread isolate.
   * @param size                  Allocation size.
   */
  inline constexpr Address allocate(Runtime::Isolate *isolate, size_t size) { return m_allocate(isolate, size); }

  /**
   * @brief Reserves a typed allocation on this buffer.
   * @param isolate               Thread isolate.
   * @param size                  Allocation size.
   */
  template <class T> inline constexpr T *allocate(Runtime::Isolate *isolate, size_t size = 1) {
    return std::bit_cast<T *>(m_allocate(isolate, size * sizeof(T)));
  }

  /**
   * @brief Allows destroying allocations (if at the top).
   * @param size                  Size to remove.
   */
  inline constexpr void release(Address address, size_t size) { m_release(address, size); }

  /**
   * @brief Allows destroying allocations (if at the top).
   * @param size                  Size to remove.
   */
  template <class T> inline constexpr void release(const T *data, size_t size = 1) {
    m_release(std::bit_cast<Address>(data), size * sizeof(T));
  }

private:
  //  PRIVATE METHODS  //

  /// @brief Handles acquiring an underlying region.
  void m_acquire();

  /**
   * @brief Allows destroying allocations.
   * @param address               Address to remove.
   * @param size                  Allocation size.
   */
  void m_release(Address address, size_t size);

  /**
   * @brief Handles allocating memory on this buffer.
   * @param isolate               Thread isolate.
   * @param size                  Allocation size.
   */
  Address m_allocate(Runtime::Isolate *isolate, size_t size);
};

} // namespace Sabre::Heap

#endif
