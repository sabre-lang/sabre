#ifndef _SABRE_STRING_SMALL_HPP
#define _SABRE_STRING_SMALL_HPP

/// Sabre Includes
#include "sabre/string/limits.hpp"
#include "sabre/value/pointer.hpp"
#include "sabre/value/symbol.hpp"

namespace Sabre::String {

/// @brief Small String Value.
struct Small : public Value::Any {
  //  TYPEDEFS  //

  /// @brief Allow wrapper access.
  friend struct String::Any;

  //  CONSTRUCTORS  //

  /// @brief Constructs a small-string value.
  constexpr Small() : Value::Any(m_encode()) {}

  /**
   * @brief Constructs a small-string from a codepoint.
   * @param cp                    Codepoint to bind.
   */
  constexpr Small(uint32_t cp) : Small($::Encoding::UTF8::from(cp)) {}

  /**
   * @brief Handles encoding small strings.
   * @param view                  Data view to bind.
   */
  constexpr Small(const char *view) : Small({view, std::strlen(view)}) {}
  constexpr Small(const $::String::View &view) : Value::Any(m_encode(view)) {}

  //  PUBLIC METHODS  //

  inline constexpr size_t size() const noexcept { return m_size(m_pointer); }
  inline constexpr size_t bytes() const noexcept { return m_bytes(m_pointer); }

  inline constexpr const char *data() const noexcept { return m_decode(this); }
  inline constexpr Value::Symbol symbol() const { return Value::Symbol(view()); }
  inline constexpr $::String::View view() const noexcept { return {data(), bytes()}; }

  /**
   * @brief Gets the offset from a given index.
   * @param unit                  Unit index to resolve.
   */
  inline constexpr size_t offset(size_t unit) const {
    if (size() == bytes()) return unit; // ASCII sizing
    return $::Encoding::UTF8::offset(data(), bytes(), unit);
  }

  /**
   * @brief Handles getting a rune value.
   * @param unit              Code-point index.
   */
  inline constexpr uint32_t rune(size_t unit) const {
    if (size() == bytes()) return data()[unit]; // ASCII
    return $::Encoding::UTF8::rune(data(), bytes(), unit);
  }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Validates the value.
   * @param pointer             Pointer to validate.
   */
  static inline constexpr bool m_is(const Pointer::Tagged &pointer) { return pointer.is<Pointer::Kind::STR>(); }

  /**
   * @brief Get truthiness of a value.
   * @param self                Value to resolve.
   */
  static inline constexpr bool m_truthiness(const Small &self) { return self.size() != 0; }

  /// @brief Constructs an empty string pointer.
  static inline constexpr Pointer::Underlying m_encode() {
    return static_cast<Pointer::Underlying>(Pointer::Kind::STR);
  }

  /**
   * @brief Handles encoding a string value.
   * @param view                  String to encode.
   */
  static inline constexpr Pointer::Underlying m_encode(const $::String::View &view) {
    $_ASSERT(view.size() <= Limits::SMALL);
    Pointer::Underlying pointer = 0; // result value
    char *base = std::bit_cast<char *>(&pointer) + 1;
    std::memcpy(std::bit_cast<void *>(base), view.data(), view.size());
    pointer |= (view.size() << Pointer::Shift::LEN) & Pointer::Mask::LAST;
    return pointer | static_cast<Pointer::Underlying>(Pointer::Kind::STR);
  }

  /**
   * @brief Handles decoding the string-size.
   * @param pointer               Pointer to decode.
   */
  static inline constexpr size_t m_size(Pointer::Underlying pointer) {
    return $::Encoding::UTF8::count(std::bit_cast<const char *>(&pointer) + 1, m_bytes(pointer));
  }

  /**
   * @brief Handles decoding the string-size.
   * @param pointer               Pointer to decode.
   */
  static inline constexpr size_t m_bytes(Pointer::Underlying pointer) {
    return (pointer & Pointer::Mask::SLEN) >> Pointer::Shift::LEN;
  }

  /**
   * @brief Handles decoding string bytes.
   * @param self                  String to decode.
   */
  static inline constexpr const char *m_decode(const Value::Any *self) {
    auto base = std::bit_cast<Pointer::Underlying>(self);
    return std::bit_cast<const char *>(base + 1);
  }

  /**
   * @brief Handles dumping the runtime value.
   * @param os                    Output stream.
   * @param self                  Value instance.
   */
  static inline void m_print(std::ostream &os, const Small &self) { os << self.view(); }
};

} // namespace Sabre::String

#endif
