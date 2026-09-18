#ifndef _SABRE_VALUE_POINTER_HPP
#define _SABRE_VALUE_POINTER_HPP

/// Sabre Includes
#include "sabre/forward/number.hpp"
#include "sabre/forward/shape.hpp"
#include "sabre/value/feedback.hpp"

/**
 *
 * The tagged-pointer scheme supports the following value types:
 *
 * DDDDDDDD DDDDDDDD DDDDDDDD DDDDDDDD DDDDDDDD DDDDDDDD DDDDDDDD DDDDDDD 0     NUM
 * xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxB xxxx 0001     BIT
 * SSSSSSSS SSSSSSSS SSSSSSSS SSSSSSSS SSSSSSSS SSSSSSSS SSSSSSSS SSSS 0011     SYM
 * SSSSSSSS SSSSSSSS SSSSSSSS SSSSSSSS SSSSSSSS SSSSSSSS SSSSSSSS LLLL 0101     STR
 * xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxx 0111     -
 * xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxx 1001     OBJ (JUNIOR)
 * xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxx 1011     OBJ (SENIOR)
 * xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxx 1101     -
 * xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx EEEE 1111     NIL
 *
 * This scheme supports immediate 63-bit integers, which are denoted by a LSB of 0.
 * Values that then have a LSB of 1 are either pointers or other immediate values. The
 * other immediate values available are:
 *
 *  NUM  |  63-bit IEEE 754 double-precision floats. The last bit of the mantissa is
 *          trimmed off when encoding, resulting in a small loss of precision, but no
 *          loss in range at all.
 *
 *  SYM  |  Unique symbol (hash-value).
 *  OBJ  |  Pointer to heap-allocated objects.
 *  BIT  |  Booleans encoded as either a true/false value.
 *  STR  |  Small strings encoded into 7-bytes (can only be valid UTF-8).
 *  NIL  |  Singleton "Void" type. Can also carry a 1-byte feedback code.
 *
 * Note: There are currently two variants that are still available for use. This could
 *       be useful for encoding "raw" pointers for FFI values or elsewise.
 *
 */

namespace Sabre::Pointer {

/// @brief Underlying Pointer Typing.
using Underlying = uintptr_t;

/// @brief Validate the incoming pointer size.
static_assert(sizeof(Number::Floating) == sizeof(Underlying));

/// @brief Associated Pointer Kinds.
enum class Kind : uint8_t {
  NUM = 0b0000,
  BIT = 0b0001,
  SYM = 0b0011,
  STR = 0b0101,
  OBJ = 0b1001,

  JUN = OBJ | 0b0000,
  SEN = OBJ | 0b0010,

  NIL = 0b1111,
};

/// @brief Available mask values.
namespace Mask {
static constexpr Underlying NUM = 0x01;  // Masks number values.
static constexpr Underlying IMM = 0x0F;  // Masks immediates.
static constexpr Underlying SLEN = 0xF0; // Masks string-length.
static constexpr Underlying LAST = 0xFF; // Masks last byte.

// allows checking for errored values
static constexpr Underlying ERRC =
    (static_cast<Underlying>(Value::Feedback::ERROR_THROWN) << 4) | static_cast<Underlying>(Kind::NIL);
}; // namespace Mask

/// @brief Available shift values.
namespace Shift {
static constexpr int32_t NUM = 1; // Number decode shift.
static constexpr int32_t OBJ = 1; // Object flag shift.
static constexpr int32_t FBK = 4; // Void/Error shift.
static constexpr int32_t SYM = 4; // Symbol decode shift.
static constexpr int32_t LEN = 4; // Length decode shift.
static constexpr int32_t BIT = 8; // Boolean decode shift.
static constexpr int32_t STR = 8; // Small string decode shift.
}; // namespace Shift

/// @brief Expected value alignments.
namespace Alignment {
static constexpr size_t VALUE = sizeof(Underlying);    // Size of pointer.
static constexpr size_t OBJECT = 1 + (UINT8_MAX >> 4); // Aligned to half-byte.
}; // namespace Alignment

/// @brief Pointer Traits Details.
class Tagged {
  //  PROPERTIES  //

  /// @brief Associated tagged value.
  Underlying m_value = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a tagged pointer.
   * @param value                   Pointer value.
   */
  constexpr Tagged() : Tagged(Kind::NIL) {}
  constexpr Tagged(Underlying value) : m_value(value) {}
  constexpr Tagged(Kind kind) : m_value(static_cast<Underlying>(kind)) {}

  //  OPERATOR METHODS  //

  /// @brief Allow direct conversion to the underlying type.
  inline constexpr operator Underlying() const noexcept { return m_value; }

  /// @brief Tagged pointers can simply be checked against one another.
  inline constexpr bool operator==(const Tagged &other) const noexcept { return m_value == other.m_value; }

  //  PUBLIC METHODS  //

  /// @brief Gets the underlying tagged value.
  inline constexpr Underlying value() const noexcept { return m_value; }

  /// @brief Gets the underlying shape value (builtin only).
  inline constexpr Shape::Underlying shape() const noexcept { return m_shape(); }

  /// @brief Denotes if a pointer is an "okay" value.
  inline constexpr bool okay() const noexcept { return (m_value & Mask::ERRC) != Mask::ERRC; }

  /// @brief Denotes if a pointer is a "sentinel" value.
  inline constexpr bool sentinel() const noexcept { return feedback() == Value::Feedback::ITER_SENTINEL; }

  /// @brief Denotes if the pointer is a "nullptr" value.
  inline constexpr bool nullish() const noexcept { return is<Kind::JUN, Kind::SEN>() && (m_value & ~Mask::IMM) == 0; }

  /// @brief Checks for associated tags.
  template <Kind... As> inline constexpr bool is() const noexcept { return ((kind() == As) || ...); }

  /// @brief Gets the underlying tagged value kind.
  inline constexpr Kind kind() const noexcept {
    return static_cast<Kind>((m_value & Mask::NUM) * (m_value & Mask::IMM));
  }

  /// @brief Gets the current associated feedback value.
  inline constexpr Value::Feedback feedback() const noexcept {
    return static_cast<Value::Feedback>(is<Kind::NIL>() * (m_value >> Shift::FBK));
  }

  /// @brief Resolves pointer kinds to labels.
  inline constexpr $::String::View label() const noexcept {
    switch (kind()) {
    case Kind::NUM: return "NUM";
    case Kind::BIT: return "BIT";
    case Kind::SYM: return "SYM";
    case Kind::STR: return "STR";
    case Kind::JUN: return "JUN";
    case Kind::SEN: return "SEN";
    default: return "UNK";
    }
  }

protected:
  //  PRIVATE METHODS  //

  /// @brief Gets a values underlying shape (not class-shape).
  Shape::Underlying m_shape() const noexcept;

  /**
   * @brief Handles printing pointers.
   * @param os                        Output stream.
   * @param self                      Pointer value.
   */
  static inline void m_print(std::ostream &os, const Tagged &self) {
    os << fmt::format("0x{0:0X} ({1})", self.m_value, self.label());
  }
};

} // namespace Sabre::Pointer

#endif
