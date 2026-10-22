#ifndef _XTDLIB_ENUM_FLAGS_HPP
#define _XTDLIB_ENUM_FLAGS_HPP

/// Library Includes
#include "xtdlib/container/flags.hpp"

namespace $::Enum {

/// @brief Enum Flags Container.
template <class E> struct Flags : public Bit::Flags<std::underlying_type_t<E>> {
  //  TYPEDEFS  //

  /// @brief Base Flags Class Alias.
  using Base = Bit::Flags<std::underlying_type_t<E>>;

  /// @brief Ensure that we have a scoped enumeration given.
  static_assert(std::is_scoped_enum_v<E>, "Expected a scoped enumeration");

  //  CONSTRUCTORS  //

  /// @brief Allow default construction.
  constexpr Flags() = default;

  /// @brief Allow direct conversion from the underlying type.
  constexpr Flags(const Base &other) : Base(other) {}

  /**
   * @brief Sets an initial set of flags.
   * @param flags                 Flags to set.
   */
  template <std::same_as<E>... Es> constexpr Flags(const Es &...flags) {
    if constexpr (sizeof...(Es)) Base::set(static_cast<Flags::Underlying>(flags)...);
  }

  //  PUBLIC METHODS  //

  /**
   * @brief Handles fliping singular bits.
   * @param flag                      Flag to flip.
   * @param state                     Optional state.
   */
  inline constexpr Flags &flip(E flag) { return Base::flip(static_cast<Flags::Underlying>(flag)), *this; }
  inline constexpr Flags &flip(E flag, bool state) {
    return Base::flip(static_cast<Flags::Underlying>(flag), state), *this;
  }

  /**
   * @brief Handles setting all the given bits.
   * @param flags                     Flags to set.
   */
  template <std::same_as<E>... Es> inline constexpr Flags &set(const Es &...flags) noexcept {
    return Base::set(static_cast<Flags::Underlying>(flags)...), *this;
  }

  /**
   * @brief Handles clearing all the given bits.
   * @param flags                     Flags to set.
   */
  template <std::same_as<E>... Es> inline constexpr Flags &clear(const Es &...flags) noexcept {
    return Base::clear(static_cast<Flags::Underlying>(flags)...), *this;
  }

  /**
   * @brief Tests if flags are set.
   * @param flags                     Flags to test.
   */
  template <std::same_as<E>... Es> inline constexpr bool test(const Es &...flags) const noexcept {
    return Base::test(static_cast<Flags::Underlying>(flags)...);
  }
};

} // namespace $::Enum

#endif
