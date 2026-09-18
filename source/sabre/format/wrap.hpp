#ifndef _SABRE_FORMAT_WRAP_HPP
#define _SABRE_FORMAT_WRAP_HPP

/// Sabre Includes
#include "sabre/format/options.hpp"

namespace Sabre::Format {

/// @brief Wrapping State.
struct Wrap {
  //  TYPEDEFS  //

  /// @brief Wrapped List of Groups.
  using List = $::Map::Set<Sigil>;

  /// @brief Potential wrap states.
  enum class State : uint8_t { DISABLED, ENABLED, DETECT, FORCE };

private:
  //  PROPERTIES  //

  /// @brief Current wrap state.
  State m_state = State::ENABLED;

public:
  //  CONSTRUCTORS  //

  /// @brief Default constructor.
  constexpr Wrap() = default;

  /**
   * @brief Constructs a wrap state.
   * @param state                 State to encapsulate.
   */
  constexpr Wrap(const State &state) : m_state(state) {}

  //  OPERATOR METHODS  //

  /// @brief Allow setting the direct state.
  inline constexpr Wrap &operator=(const State &state) { return m_state = state, *this; }

  //  PUBLIC METHODS  //

  /// @brief Gets the underlying wrapping details.
  inline constexpr bool forced() const noexcept { return m_state == State::FORCE; }
  inline constexpr bool enabled() const noexcept { return m_state == State::ENABLED; }
  inline constexpr bool disabled() const noexcept { return m_state == State::DISABLED; }

  /// @brief Constructs an auto-resolved state.
  inline constexpr Wrap resolve() const noexcept { return disabled() ? *this : State::ENABLED; }

  /// @brief Constructs an auto-detection wrap state.
  inline constexpr Wrap detect() const noexcept { return disabled() ? *this : State::DETECT; }
};

} // namespace Sabre::Format

#endif
