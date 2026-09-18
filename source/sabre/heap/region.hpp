#ifndef _SABRE_HEAP_REGION_HPP
#define _SABRE_HEAP_REGION_HPP

/// Sabre Includes
#include "sabre/heap/options.hpp"
#include "sabre/object/header.hpp"

namespace Sabre::Heap {

/// @brief Available Region Generations.
enum class Epoch : uint8_t {
  FREE,   // Unused heap regions.
  EDEN,   // Young region objects.
  JUNIOR, // Survived GC once.
  SENIOR, // Long-lived objects.
  STACK,  // Frame stack allocation.
};

/// @brief Heap Callback Handler.
using Callback = $::Shared::Functor<void(Object::Header *)>;

/// @brief Region of Heap Memory.
class $_ALIGNAS(Pointer::Alignment::OBJECT) Region {
  //  TYPEDEFS  //

  /// @brief Allow heap service internal access.
  friend class Service;

  //  PROPERTIES  //

  /// @brief The current usage value.
  size_t m_used = 0;

  /// @brief The encoded parent/generation.
  Address m_encoded = m_merge(Epoch::FREE);

public:
  //  CONSTRUCTORS  //

  /// @brief Default constructor for regions.
  constexpr Region() = default;

  //  PUBLIC METHODS  //

  /// @brief Gets the total used bytes for this region.
  inline constexpr size_t used() const noexcept { return m_used; }

  /// @brief Gets the encoded region generation.
  inline constexpr Epoch epoch() const { return static_cast<Epoch>(m_encoded & Pointer::Mask::IMM); }

  /// @brief Gets the encoded parent region.
  inline constexpr Region *parent() const { return std::bit_cast<Region *>(m_encoded & ~Pointer::Mask::IMM); }

  /// @brief Gets the current remaining memory.
  inline constexpr size_t remaining() const { return m_maximum() - m_used; }

  /// @brief Gets the associated region label identifier.
  inline constexpr size_t label() const { return (m_address() % Limits::ADDRESS_SPACE) / Limits::REGIONS_SIZE; }

  /// @brief Gets the current top of the region.
  inline constexpr Address head() const { return buffer() + m_used; }

  /// @brief Gets the address of the underlying buffer.
  inline constexpr Address buffer() const { return m_address() + sizeof(Region); }

  /** Handles clearing the region it encompasses. */
  inline void reset() noexcept { m_encoded = m_merge(Epoch::FREE, parent()), m_used = 0; }

  /**
   * @brief Determines if memory fits within the region.
   * @param size                Size to be checked.
   */
  inline constexpr bool fits(size_t size) const noexcept { return size <= remaining(); }

  /**
   * @brief Handles promoting regions.
   * @param epoch               Region generation.
   * @param parent              Parent to inherit.
   */
  inline Epoch promote(Epoch epoch) noexcept { return (m_encoded = m_merge(epoch, parent())), epoch; }
  inline Epoch promote(Epoch epoch, Region *parent) noexcept { return (m_encoded = m_merge(epoch, parent)), epoch; }

  /**
   * @brief Releases memory from this region.
   * @param size                Size to releae.
   */
  inline constexpr size_t release(size_t size) { return $_ASSERT(size <= m_used), m_used -= size; }

  /**
   * @brief Handles allocating memory on this region.
   * @param size                  Size of memory.
   */
  inline constexpr Address allocate(size_t size) {
    // pre-ensure the heap allocation will succeed
    $_ASSERT(fits(size), "Allocation of {0} bytes does not fit region", size);

    // prepare the starting and ending addresses
    Address start = buffer() + m_used;
    $_UNUSED Address end = start + size;

    // ensure some conditions are valid before continuing
    $_ASSERT(start % Pointer::Alignment::OBJECT == 0, "Allocation start-alignment failed");
    $_ASSERT(end % Pointer::Alignment::OBJECT == 0, "Allocation end-alignment failed");
    $_ASSERT(end <= buffer() + m_maximum(), "Allocation exceeds maximum region bounds");

    // increment the used space and return the allocation position
    return m_used += size, start;
  }

  /**
   * @brief Handles iterating over each region item.
   * @param callback              Callback to execute.
   */
  inline void each(Callback &&callback) const noexcept {
    for (Address offset = buffer(); offset < m_used;) {
      auto *header = std::bit_cast<Object::Header *>(offset);
      callback(header), offset += header->size(); // bump up
    }
  }

private:
  //  PRIVATE METHODS  //

  /// @brief Gets the address of the region.
  inline constexpr Address m_address() const noexcept { return std::bit_cast<Address>(this); }

  /// @brief Maximum allowable capacity.
  static inline constexpr size_t m_maximum() { return Limits::REGIONS_SIZE - sizeof(Region); }

  /**
   * @brief Encodes the generation and parent pointer.
   * @param epoch               Epoch to encode.
   * @param parent              Parent to encode.
   */
  static inline constexpr Address m_merge(Epoch epoch, Region *parent = nullptr) {
    return std::bit_cast<Address>(parent) | static_cast<Address>(epoch);
  }
};

} // namespace Sabre::Heap

#endif
