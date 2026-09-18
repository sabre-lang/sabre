#ifndef _SABRE_NUMBER_TAGGED_HPP
#define _SABRE_NUMBER_TAGGED_HPP

/// Sabre Includes
#include "sabre/number/math.hpp"
#include "sabre/value/common.hpp"

//  MACROS  //

#define MM_APPLY_UNOP(OP, V, ...)                                         \
  inline constexpr Tagged operator OP() const noexcept { return OP V(); }

#define MM_APPLY_BINOP(OP, V, ...)                                                                     \
  inline constexpr Tagged operator OP(const Tagged &other) const noexcept { return V() OP other.V(); }

//  NAMESPACES  //

namespace Sabre::Number {

/// @brief Tagged Numeric Value.
struct Tagged : public Value::Any {
  //  CONSTRUCTORS  //

  /// @brief Defaults to a zero-value.
  constexpr Tagged() : Tagged(0.0f) {}

  /**
   * @brief Constructs a numeric.
   * @param value                 Value to bind.
   */
  template <std::floating_point T> constexpr Tagged(T value) : Value::Any(m_encode(value)) {}

  /**
   * @brief Constructs a numeric.
   * @param value                 Value to bind.
   */
  template <std::integral T> constexpr Tagged(T value) : Value::Any(m_encode(static_cast<Floating>(value))) {}

  //  OPERATOR METHODS  //

  inline constexpr operator float() const noexcept { return static_cast<float>(value()); }
  inline constexpr operator double() const noexcept { return static_cast<double>(value()); }
  inline constexpr operator size_t() const noexcept { return static_cast<size_t>(value()); }
  inline constexpr operator int32_t() const noexcept { return static_cast<int32_t>(value()); }
  inline constexpr operator int64_t() const noexcept { return static_cast<int64_t>(value()); }

  inline constexpr bool operator==(double value) const noexcept { return m_pointer == Tagged(value).m_pointer; }
  inline constexpr bool operator!=(double value) const noexcept { return m_pointer != Tagged(value).m_pointer; }
  inline constexpr auto operator<=>(const Tagged &other) const noexcept { return value() <=> other.value(); }

  MM_APPLY_UNOP(-, value)
  MM_APPLY_UNOP(~, m_integral)

  MM_APPLY_BINOP(+, value)
  MM_APPLY_BINOP(-, value)
  MM_APPLY_BINOP(*, value)
  MM_APPLY_BINOP(/, value)

  MM_APPLY_BINOP(|, m_integral)
  MM_APPLY_BINOP(&, m_integral)
  MM_APPLY_BINOP(^, m_integral)
  MM_APPLY_BINOP(<<, m_integral)
  MM_APPLY_BINOP(>>, m_integral)

  inline constexpr Tagged operator%(const Tagged &other) const noexcept { return std::fmod(value(), other.value()); }

  //  PUBLIC METHODS  //

  /// @brief Gets the underlying numeric value.
  inline constexpr Floating value() const noexcept { return m_decode(m_pointer); }

  /// @brief Denotes if the value is an integral.
  inline constexpr bool integral() const noexcept { return Math::integral(value()); }

  /**
   * @brief Handles comparing two numbers.
   * @param other                 Other value.
   */
  inline constexpr int32_t compare(const Tagged &other) const noexcept {
    return Math::compare(m_decode(m_pointer), m_decode(other.m_pointer));
  }

  /**
   * @brief Calculates the power-of a value.
   * @param exponential           Exponential value.
   */
  inline constexpr Tagged power(const Tagged &exponential) const noexcept {
    return std::pow(value(), exponential.value());
  }

protected:
  //  PRIVATE METHODS  //

  /// @brief Casts the underlying value to an integral.
  inline constexpr Integral m_integral() const noexcept { return static_cast<Integral>(value()); }

  /**
   * @brief Validates the value.
   * @param pointer             Tagged pointer.
   */
  static inline constexpr bool m_is(const Pointer::Tagged &pointer) { return pointer.is<Pointer::Kind::NUM>(); }

  /**
   * @brief Get truthiness of a value.
   * @param self                Value to resolve.
   */
  static inline constexpr bool m_truthiness(const Tagged &self) { return self.m_pointer.value() != 0; }

  /**
   * @brief Handles encoding numbers.
   * @param value               Value to encode.
   */
  static inline constexpr Pointer::Underlying m_encode(Floating value) {
    auto bits = std::bit_cast<Pointer::Underlying>(value) & ~Pointer::Mask::NUM;
    return bits | static_cast<Pointer::Underlying>(Pointer::Kind::NUM);
  }

  /**
   * @brief Handles decoding numbers.
   * @param pointer             Pointer to decode.
   */
  static inline constexpr Floating m_decode(Pointer::Underlying pointer) {
    $_ASSERT(m_is(pointer), "Invalid 'Number' decode value");
    return std::bit_cast<Floating>(pointer & ~Pointer::Mask::NUM);
  }

  /**
   * @brief Handles dumping the runtime value.
   * @param os                  Output stream.
   * @param self                Value instance.
   */
  static inline void m_print(std::ostream &os, const Tagged &self) { os << $::Dye::yellow(self.value()); }
};

/// @brief Gets a zero value.
static inline constexpr Tagged Zero = Tagged(0);

/// @brief Gets a one value.
static inline constexpr Tagged One = Tagged(1);

/// @brief Gets the Not-A-Number value.
static inline constexpr Tagged Invalid = Tagged(NAN);

/// @brief Gets the "infinity" value.
static inline constexpr Tagged Infinity = Tagged(INFINITY);

} // namespace Sabre::Number

//  UNDEFINES  //

#undef MM_APPLY_UNOP
#undef MM_APPLY_BINOP

#endif
