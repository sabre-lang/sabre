#ifndef _SABRE_ITERABLE_BUFFER_HPP
#define _SABRE_ITERABLE_BUFFER_HPP

/// Sabre Includes
#include "sabre/iterable/iterator.hpp"

namespace Sabre {

/// @brief Buffer Attributes.
template <> struct Object::Wrapper<Iterable::Buffer> {
  //  PROPERTIES  //

  /// @brief The bounded buffer value.
  $::Memory::Region region = {};

  //  CONSTRUCTORS  //

  /// @brief Constructs a defaulted region of memory.
  explicit Wrapper() = default;

  /**
   * @brief Constructs an empty/sized list.
   * @param capacity              Initial capacity.
   */
  explicit Wrapper(size_t capacity, uint8_t fill = 0) : region(capacity) { std::ranges::fill(region.span(), fill); }

  /**
   * @brief Handles copying a region to be used.
   * @param region                Region to bind.
   */
  explicit Wrapper($::Memory::Region &&region) : region(std::move(region)) {}

  /**
   * @brief Constructs a list.
   * @param elements              Elements to bind.
   */
  explicit Wrapper(const std::vector<uint8_t> &elements) : region(elements) {}

  /**
   * @brief Constructs a list.
   * @param elements              Elements to bind.
   */
  explicit Wrapper(const std::span<uint8_t> &elements) : region(elements) {}
  explicit Wrapper(const std::span<const uint8_t> &elements) : region(elements) {}
};

/// @brief Buffer Interface.
struct Iterable::Buffer : public Object::Mixin<Iterable::Buffer> {
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Mixin::Mixin;

  //  PUBLIC METHODS  //

  /// @brief Denotes if the list is empty.
  inline constexpr bool empty() const { return m_wrapper()->region.empty(); }

  /// @brief Gets the size of the list.
  inline constexpr size_t size() const { return m_wrapper()->region.size(); }

  /// @brief Gets the available values from the list.
  inline constexpr uint8_t *data() const { return static_cast<uint8_t *>(m_wrapper()->region.data()); }

  /// @brief Gets a view of the internal buffer span.
  inline constexpr std::span<uint8_t> span() const { return m_wrapper()->region.span(); }

  /// @brief Gets the front-most value.
  inline constexpr uint8_t front() const { return get(0); }

  /// @brief Gets the back-most value.
  inline constexpr uint8_t back() const { return get(size() - 1); }

  /**
   * @brief Allow slicing spans.
   * @param offset                Offset to slice.
   * @param count                 Total slice count.
   */
  inline constexpr std::span<uint8_t> slice(size_t offset, size_t count = std::dynamic_extent) const {
    return span().subspan(offset, count);
  }

  /**
   * @brief Handles getting a list-value.
   * @param index                 Index of value.
   */
  inline constexpr uint8_t get(size_t index) const noexcept {
    $_UNUSED $_AUTO = m_guard(index);
    return data()[index]; // resolve
  }

  /**
   * @brief Handles setting a list-value.
   * @param index                 Index of value.
   * @param value                 Value to set.
   */
  inline constexpr uint8_t set(size_t index, const uint8_t &value) const noexcept {
    $_UNUSED $_AUTO = m_guard(index);
    return data()[index] = value;
  }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Prepares a suitably guarded handler.
   * @param index                 Index to validate.
   */
  inline constexpr Object::Guard m_guard($_UNUSED size_t index) const noexcept {
    return $_ASSERT(index < size(), "Index {0} exceeds list-size {1}", index, size()), Mixin::m_guard();
  }

  /**
   * @brief Handles printing values.
   * @param os                    Output stream.
   * @param self                  Buffer instance.
   */
  static inline void m_print(std::ostream &os, const Buffer &self) {
    os << $::Dye::cyan("<{0}: size({1})>", self.brand(), self.size());
  }
};

} // namespace Sabre

#endif
