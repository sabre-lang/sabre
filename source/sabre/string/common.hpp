#ifndef _SABRE_STRING_COMMON_HPP
#define _SABRE_STRING_COMMON_HPP

/// Sabre Includes
#include "sabre/string/large.hpp"
#include "sabre/string/small.hpp"

namespace Sabre::String {

/// @brief Define an explicit string that is empty.
static inline constexpr Small Empty = {};

/// @brief Dynamic String Pointer.
struct Any : public Value::Any {
  //  CONSTRUCTORS  //

  /// @brief Constructs a defaulted string instance.
  constexpr Any() : Value::Any(Empty) {}

  /**
   * @brief Handles conversion from derived strings.
   * @param target                    Target to encapsulate.
   */
  constexpr Any(const Small &target) : Value::Any(target) {}
  constexpr Any(const Large &target) : Value::Any(target) {}

  /**
   * @brief Constructs any string.
   * @param isolate                   Runtime isolate.
   * @param intern                    Interned string.
   */
  explicit Any(Runtime::Isolate *isolate, const Intern &intern);

  /**
   * @brief Constructs any string.
   * @param isolate                   Runtime isolate.
   * @param value                     String value to bind.
   */
  explicit Any(Runtime::Isolate *isolate, const $::String::View &value);

  /**
   * @brief Constructs formatted strings.
   * @param isolate                   Runtime isolate.
   * @param fmt                       String format.
   * @param args                      Format arguments.
   */
  template <class... As>
  explicit Any(Runtime::Isolate *isolate, const $::String::View &fmt, As &&...args) :
      Any(isolate, fmt::format(fmt::runtime(fmt), std::forward<As>(args)...)) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the size of a string (code-points).
  inline constexpr size_t size() const noexcept {
    return Small::m_is(m_pointer) ? Small::m_size(m_pointer) : m_as<Large>()->size();
  }

  /// @brief Gets the size of a string (bytes).
  inline constexpr size_t bytes() const noexcept {
    return Small::m_is(m_pointer) ? Small::m_bytes(m_pointer) : m_as<Large>()->bytes();
  }

  /// @brief Gets the associated data to be used.
  inline constexpr const char *data() const noexcept {
    return Small::m_is(m_pointer) ? Small::m_decode(this) : m_as<Large>()->data();
  }

  /// @brief Gets the view of the string.
  inline constexpr $::String::View view() const noexcept { return {data(), bytes()}; }

  /// @brief Gets the symbol value of the string.
  inline constexpr Value::Symbol symbol() const {
    if (Large::m_is(m_pointer)) return m_as<Large>()->symbol(); // faster
    return Value::Symbol(Small::m_decode(this), Small::m_bytes(m_pointer));
  }

  /**
   * @brief Gets the offset from a given index.
   * @param unit                        Unit index to resolve.
   */
  inline constexpr size_t offset(size_t unit) const {
    // resolve with large string if necessary
    if (Large::m_is(m_pointer)) return m_as<Large>()->offset(unit);

    // otherwise we want to decipher the necessary offset
    auto data = Small::m_decode(this);
    auto bytes = Small::m_bytes(m_pointer);

    // if same, then same as ASCII
    if (bytes == Small::m_size(m_pointer)) return unit;
    return $::Encoding::UTF8::offset(data, bytes, unit);
  }

  /**
   * @brief Gets the rune at a given unit.
   * @param unit                        Unit to resolve codepoint.
   */
  inline constexpr uint32_t rune(size_t unit) const {
    // resolve with large string if necessary
    if (Large::m_is(m_pointer)) return m_as<Large>()->rune(unit);

    // get the current sizes to be compared
    auto data = Small::m_decode(this);
    auto bytes = Small::m_bytes(m_pointer);

    // if same, then we have ASCII only
    if (bytes == Small::m_size(m_pointer)) return static_cast<uint32_t>(data[unit]);

    // otherwise attempt resolving the incoming rune now
    return $::Encoding::UTF8::rune(data, bytes, unit);
  }

  /**
   * @brief Handles comparing to another value.
   * @param right                       Right value to compare.
   */
  inline constexpr int32_t compare(const $::String::View &right) const noexcept { return view().compare(right); }
  inline constexpr int32_t compare(const String::Any &right) const noexcept { return view().compare(right.view()); }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles validating string pointers.
   * @param pointer                     Pointer to check.
   */
  static inline constexpr bool m_is(const Pointer::Tagged &pointer) {
    return Small::m_is(pointer) || Large::m_is(pointer);
  }

  /**
   * @brief Gets the truthiness of a value.
   * @param self                        String instance.
   */
  static inline constexpr bool m_truthiness(const Any &self) { return self.size() != 0; }

  /**
   * @brief Handles dumping the runtime value.
   * @param os                          Output stream.
   * @param self                        Value instance.
   */
  static inline void m_print(std::ostream &os, const Any &self) { os << self.view(); }
};

} // namespace Sabre::String

#endif
