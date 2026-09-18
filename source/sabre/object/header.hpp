#ifndef _SABRE_OBJECT_HEADER_HPP
#define _SABRE_OBJECT_HEADER_HPP

/// Sabre Includes
#include "sabre/forward/object.hpp"
#include "sabre/heap/limits.hpp"
#include "sabre/value/common.hpp"

namespace Sabre::Object {

/// @brief Available Object Flags.
enum class Flag : uint8_t { HASHED, YOUNG, REACHABLE };

/// @brief Object Header.
class $_ALIGNAS(Pointer::Alignment::OBJECT) Header {
  //  PROPERTIES  //

  mutable $::Mutex::Light m_mutex = 0; // Small object mutex.           1

  $::Async::Atomic<uint8_t> m_size = 0;      // Allocation size.        1
  $::Async::Atomic<uint8_t> m_flags = 0;     // Object flags.           1
  $::Async::Atomic<uint8_t> m_survivors = 0; // GC survivals.           1

  $::Async::Atomic<uint32_t> m_forwarded = 0;  // Forwarded object.     4
  $::Async::Atomic<Shape::Underlying> m_shape; // Object archetype.     8

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a header instance.
   * @param shape                 Shape to bind.
   * @param size                  Size of object.
   */
  explicit Header(Shape::Underlying shape, size_t size) : m_size(size), m_shape(shape) {
    toggle<Flag::YOUNG>(true); // set the initial flag to being young now
  }

  //  PUBLIC METHODS  //

  /// @brief Gets the underlying mutex value.
  inline constexpr $::Mutex::Light &mutex() { return m_mutex; }
  inline constexpr bool locked() const noexcept { return m_mutex.state(); }

  /// @brief Gets the allocation size.
  inline constexpr uint8_t size() const noexcept { return m_size; }

  /// @brief Gets the associated shaped value.
  inline constexpr Shape::Underlying shape() const noexcept { return m_shape; }

  /// @brief Gets the underlying survivor count.
  inline constexpr uint8_t survivors() noexcept { return m_survivors; }

  /// @brief Denotes if the header is reachable.
  inline constexpr bool reachable() const noexcept { return is<Flag::REACHABLE>(); }

  /// @brief Constructs a view-only set of flags.
  inline constexpr $::Enum::Flags<Flag> flags() const noexcept { return static_cast<Flag>(m_flags.load()); }

  /// @brief Allow getting the underlying region of a header.
  inline constexpr Heap::Region *region() const noexcept {
    return std::bit_cast<Heap::Region *>(address() & Heap::Limits::REGIONS_MASK);
  }

  /// @brief Gets the underlying header address.
  inline constexpr Pointer::Underlying payload() const noexcept { return address() + sizeof(Header); }
  inline constexpr Pointer::Underlying address() const noexcept { return std::bit_cast<Pointer::Underlying>(this); }

  /// @brief Gets the forwarded header value.
  inline constexpr uint32_t forwarded() const noexcept { return m_forwarded; }
  inline constexpr uint32_t forwarded(uint32_t encoded) noexcept { return m_forwarded = encoded; }

  /// @brief Handles promoting headers to junior/senior regions.
  inline constexpr void promote(bool senior) {
    toggle<Flag::REACHABLE>(false);
    toggle<Flag::YOUNG>(!senior);
    if (senior) m_survivors = 0;
  }

  /// @brief Handles encoding an object instance.
  template <std::derived_from<Value::Any> T = Value::Any> inline constexpr T encode() const noexcept {
    static constexpr auto kind = static_cast<Pointer::Underlying>(Pointer::Kind::OBJ);
    return Value::Any(payload() | kind | (!is<Flag::YOUNG>() << Pointer::Shift::OBJ)).as<T>();
  }

  /// @brief Checks if a value is a particular shape.
  template <std::derived_from<Value::Any>... As> inline constexpr bool is() const noexcept {
    return ((m_shape == Shape::Lookup<As>()) || ...);
  }

  /// @brief Checks if a given flag is assigned.
  template <Flag... Fs> inline constexpr bool is() const noexcept { return flags().test(Fs...); }

  /// @brief Handles toggling a header flag.
  template <Flag F> inline constexpr bool toggle(bool state) {
    auto value = flags(); // get the flags
    if (value.test(F) == state) return false;

    // update the current survivor count if necessary
    if constexpr (F == Flag::REACHABLE) m_survivors += state;

    // and return the final result when completed
    return m_flags = flags().flip(F, state).value(), true;
  }
};

/// @brief Ensure the alignment of headers is suitably value.
static_assert(sizeof(Object::Header) % Pointer::Alignment::OBJECT == 0);

} // namespace Sabre::Object

#endif
