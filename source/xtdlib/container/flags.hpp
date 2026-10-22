#ifndef _XTDLIB_CONTAINER_FLAGS_HPP
#define _XTDLIB_CONTAINER_FLAGS_HPP

/// Library Includes
#include "xtdlib/portable/stdlib.hpp"

namespace $::Bit {

/// @brief Bit Flags Container.
template <std::unsigned_integral T> struct Flags {
  //  TYPEDEFS  //

  /// @brief The underlying flags typing.
  using Underlying = T;

private:
  //  PROPERTIES  //

  /// @brief The underlying flags value.
  Underlying m_flags = 0;

public:
  //  CONSTRUCTORS  //

  /// @brief Allow default construction.
  constexpr Flags() = default;

  /**
   * @brief Sets the initial set of flags.
   * @param flags                 Flags to set.
   */
  constexpr Flags(Underlying flags) : m_flags(flags) {}

  /**
   * @brief Sets an initial set of flags.
   * @param flags                 Flags to set.
   */
  template <class... Ts> constexpr Flags(std::convertible_to<T> auto... flags) {
    if constexpr (sizeof...(Ts)) set(std::forward<Ts>(flags)...);
  }

  //  OPERATOR METHODS  //

  inline constexpr bool operator==(const Flags &other) const noexcept { return m_flags == other.m_flags; }
  inline constexpr bool operator!=(const Flags &other) const noexcept { return m_flags != other.m_flags; }

  inline constexpr Flags &operator|=(Underlying other) noexcept { return m_flags |= other, *this; }
  inline constexpr Flags &operator|=(const Flags &other) noexcept { return m_flags |= other.m_flags, *this; }

  inline constexpr Flags &operator&=(Underlying other) noexcept { return m_flags &= other, *this; }
  inline constexpr Flags &operator&=(const Flags &other) noexcept { return m_flags &= other.m_flags, *this; }

  //  PUBLIC METHODS  //

  /// @brief Denotes if no bits have been set.
  inline constexpr bool none() const noexcept { return m_flags == 0; }

  /// @brief Denotes if any bits have been set.
  inline constexpr bool any() const noexcept { return m_flags != 0; }

  /// @brief Denotes if all bits have been set.
  inline constexpr bool all() const noexcept { return m_flags == max(); }

  /// @brief Gets the current underlying value.
  inline constexpr Underlying value() const noexcept { return m_flags; }

  /// @brief Gets the maximum value possible.
  inline constexpr Underlying max() const noexcept { return std::numeric_limits<Underlying>().max(); }

  /// @brief Forcibly sets all underlying bits.
  inline constexpr Flags &set() noexcept { return m_flags = max(), *this; }

  /// @brief Forcibly clears all the underlying bits.
  inline constexpr Flags &clear() noexcept { return m_flags = 0, *this; }

  /**
   * @brief Handles fliping singular bits.
   * @param flag                      Flag to flip.
   * @param state                     Optional state.
   */
  inline constexpr Flags &flip(T flag) { return m_flags ^= m_nth(flag), *this; }
  inline constexpr Flags &flip(T flag, bool state) { return state ? set(flag) : clear(flag), *this; }

  /**
   * @brief Handles setting all the given bits.
   * @param flags                     Flags to set.
   */
  template <class... Ts> inline constexpr Flags &set(std::convertible_to<T> auto... flags) noexcept {
    return m_flags |= (m_nth(flags) | ...), *this;
  }

  /**
   * @brief Handles clearing all the given bits.
   * @param flags                     Flags to set.
   */
  template <class... Ts> inline constexpr Flags &clear(std::convertible_to<T> auto... flags) noexcept {
    return m_flags = ((m_flags & ~m_nth(flags)) | ...), *this;
  }

  /**
   * @brief Tests if flags are set.
   * @param flags                     Flags to test.
   */
  template <class... Ts> inline constexpr bool test(std::convertible_to<T> auto... flags) const noexcept {
    return ((m_flags >> flags & 1) || ...);
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Casts a flag to a bit.
   * @param flag                      Flag to cast.
   */
  inline constexpr Underlying m_nth(Underlying flag) const noexcept { return 1 << flag; }
};

} // namespace $::Bit

#endif
