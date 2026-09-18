#ifndef _SABRE_VALUE_BOOLEAN_HPP
#define _SABRE_VALUE_BOOLEAN_HPP

/// Sabre Includes
#include "sabre/value/common.hpp"

namespace Sabre::Value {

/// @brief Boolean Runtime Value.
struct Boolean : public Any {
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a boolean value.
   * @param state                     State to bind.
   */
  constexpr Boolean(bool state = false) : Any(m_encode(state)) {}

  //  OPERATOR METHODS  //

  /// @brief Gets the associated truthiness.
  inline constexpr operator bool() const noexcept { return state(); }

  //  PUBLIC METHODS  //

  /// @brief Gets the current boolean state.
  inline constexpr bool state() const { return m_decode(m_pointer); }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Validates the value.
   * @param pointer             Tagged pointer.
   */
  static inline constexpr bool m_is(const Pointer::Tagged &pointer) { return pointer.is<Pointer::Kind::BIT>(); }

  /**
   * @brief Get truthiness of a value.
   * @param self                Value to resolve.
   */
  static inline constexpr bool m_truthiness(const Boolean &self) { return self.state(); }

  /**
   * @brief Handles decoding booleans.
   * @param pointer             Pointer to decode.
   */
  static inline constexpr bool m_decode(Pointer::Underlying pointer) {
    $_ASSERT(m_is(pointer), "Invalid 'Boolean' decode value");
    return (pointer >> Pointer::Shift::BIT) != 0; // and shift
  }

  /**
   * @brief Handles encoding boolean values.
   * @param state                 State to encode.
   */
  static inline constexpr Pointer::Underlying m_encode(bool state) {
    return (state * (1 << Pointer::Shift::BIT)) | static_cast<Pointer::Underlying>(Pointer::Kind::BIT);
  }

  /**
   * @brief Handles dumping the runtime value.
   * @param os                    Output stream.
   * @param self                  Value instance.
   */
  static inline void m_print(std::ostream &os, const Boolean &self) {
    os << $::Dye::magenta(self.state() ? "True" : "False");
  }
};

/// @brief The global "True" value.
static inline constexpr auto True = Boolean(true);

/// @brief The global "False" value.
static inline constexpr auto False = Boolean(false);

} // namespace Sabre::Value

#endif
