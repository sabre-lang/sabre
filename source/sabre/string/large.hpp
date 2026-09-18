#ifndef _SABRE_STRING_LARGE_HPP
#define _SABRE_STRING_LARGE_HPP

/// Sabre Includes
#include "sabre/object/mixin.hpp"
#include "sabre/string/intern.hpp"

namespace Sabre {

/// @brief String Object Attributes.
template <> struct Object::Wrapper<String::Large> {
  //  PROPERTIES  //

  /// @brief Underlying interned storage.
  String::Intern storage = {};

  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a large string.
   * @param intern                Intern to bind.
   */
  explicit Wrapper(const String::Intern &intern) : storage(intern) {}

  /**
   * @brief Constructs a large string.
   * @param buffer                Buffer to bind.
   */
  explicit Wrapper(const $::String::View &buffer = {}) : storage(buffer) {}
};

/// @brief Large String Interface.
struct String::Large : public Object::Mixin<Large> {
  //  TYPEDEFS  //

  /// @brief Allow wrapper access.
  friend struct String::Any;

  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Mixin::Mixin;

  //  PUBLIC METHODS  //

  inline constexpr bool empty() const { return m_storage().m_runes.empty(); }
  inline constexpr size_t size() const { return m_storage().m_runes.size(); }
  inline constexpr size_t bytes() const { return m_storage().m_runes.bytes(); }
  inline constexpr const char *data() const { return m_storage().m_runes.data(); }
  inline constexpr $::String::View view() const { return m_storage().m_runes.view(); }

  /// @brief Handles getting the underlying symbol value.
  inline constexpr Value::Symbol symbol() const {
    auto &storage = m_storage(); // prepare the storage now
    auto hashed = m_header()->toggle<Object::Flag::HASHED>(true);

    // check if the incoming value has already been cached
    if ($_LIKELY(hashed)) return storage.m_symbol;
    return storage.m_symbol = storage.m_runes.symbol();
  }

  /**
   * @brief Gets the offset from a given index.
   * @param unit                  Unit index to resolve.
   */
  inline constexpr size_t offset(size_t unit) const {
    return size() == bytes() ? unit : m_storage().m_runes.units()[unit];
  }

  /**
   * @brief Reads a codepoint rune from the string.
   * @param unit                  Unit index expected.
   */
  inline constexpr uint32_t rune(size_t unit) const {
    // if the underlying bytes size is same as utf-8 size, then return immediately
    if (size() == bytes()) return static_cast<uint32_t>(data()[unit]);

    auto index = m_storage().m_runes.units()[unit];
    auto start = data() + index, end = data() + bytes();

    uint32_t codepoint; // prepare the codepoint to be read now and ensure valid
    return $_EXPECT($::Encoding::UTF8::peek(start, end, codepoint)), codepoint;
  }

protected:
  //  PRIVATE METHODS  //

  /// @brief Gets the underlying interned storage.
  inline constexpr Intern &m_storage() const noexcept { return m_wrapper()->storage; }

  /**
   * @brief Handles printing values.
   * @param os                    Output stream.
   * @param self                  Value instance.
   */
  static inline void m_print(std::ostream &os, const Large &self) { os << self.view(); }
};

} // namespace Sabre

#endif
