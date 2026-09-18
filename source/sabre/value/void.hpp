#ifndef _SABRE_VALUE_VOID_HPP
#define _SABRE_VALUE_VOID_HPP

/// Sabre Includes
#include "sabre/value/common.hpp"

namespace Sabre::Value {

/// @brief Void Runtime Value.
struct Void : public Any {
  //  CONSTRUCTORS  //

  /// @brief Constructs a default void-value.
  explicit constexpr Void() : Any(Pointer::Kind::NIL) {}

  /**
   * @brief Constructs a void value with an error.
   * @param feedback              Feedback value.
   */
  constexpr Void(const Feedback &feedback) : Any(m_encode(feedback)) {}

  //  PUBLIC METHODS  //

  /// @brief Denotes if the void value is "okay".
  inline constexpr bool okay() const noexcept { return m_pointer.okay(); }

  /// @brief Gets the underlying error value.
  inline constexpr Feedback feedback() const noexcept { return m_pointer.feedback(); }

  /// @brief Denotes if we have a sentinel value.
  inline constexpr bool sentinel() const noexcept { return feedback() == Feedback::ITER_SENTINEL; }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Get truthiness of a value.
   * @param pointer             Pointer to resolve.
   */
  static inline constexpr bool m_truthiness(const Void &) { return false; }

  /**
   * @brief Validates the value.
   * @param pointer             Tagged pointer.
   */
  static inline constexpr bool m_is(const Pointer::Tagged &pointer) { return pointer.is<Pointer::Kind::NIL>(); }

  /**
   * @brief Handles encoding error values.
   * @param code                Error code.
   */
  static inline constexpr Pointer::Underlying m_encode(const Feedback &feedback) {
    return (static_cast<uint8_t>(feedback) << Pointer::Shift::FBK) |
           static_cast<Pointer::Underlying>(Pointer::Kind::NIL);
  }

  /**
   * @brief Handles dumping the runtime value.
   * @param os                    Output stream.
   * @param self                  Value instance.
   */
  static inline void m_print(std::ostream &os, const Void &self) {
    os << $::Dye::dim("Void") << $::Dye::dim(self.okay() ? "" : "(EXCEPTION)");
  }
};

//  PUBLIC METHODS  //

static inline constexpr Value::Any Failure() { return Void(Feedback::ERROR_THROWN); }
static inline constexpr Value::Any Timeout() { return Void(Feedback::ASYNC_TIMEOUT); }
static inline constexpr Value::Any Missing() { return Void(Feedback::FIELD_MISSING); }
static inline constexpr Value::Any Sentinel() { return Void(Feedback::ITER_SENTINEL); }

} // namespace Sabre::Value

#endif
