#ifndef _SABRE_STRING_INTERN_HPP
#define _SABRE_STRING_INTERN_HPP

/// Sabre Includes
#include "sabre/string/runes.hpp"

namespace Sabre::String {

/// @brief String Intern Attributes.
class Intern {
  //  TYPEDEFS  //

  /// @brief Allow large strings internal access.
  friend struct String::Large;

  //  PROPERTIES  //

  /// @brief The available code-points.
  Runes m_runes = {};

  /// @brief Uninitialized symbol value.
  Value::Symbol m_symbol = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Defaulted string constructor.
  constexpr Intern() = default;

  /**
   * @brief Constructs an interned string.
   * @param buffer                Buffer to bind.
   */
  constexpr Intern(const $::String::View &buffer) : m_runes(buffer) {}

  /**
   * @brief Constructs an interned string.
   * @param buffer                Buffer to bind.
   * @param symbol                Pre-calcuated symbol.
   */
  constexpr Intern(const $::String::View &buffer, const Value::Symbol &symbol) : m_runes(buffer), m_symbol(symbol) {
    $_ASSERT(m_symbol == Value::Symbol(m_runes.symbol()));
  }

  //  PUBLIC METHODS  //

  inline constexpr bool empty() const noexcept { return m_runes.empty(); }
  inline constexpr size_t size() const noexcept { return m_runes.size(); }
  inline constexpr size_t bytes() const noexcept { return m_runes.bytes(); }

  inline constexpr const Runes &runes() const noexcept { return m_runes; }
  inline constexpr Value::Symbol symbol() const noexcept { return m_symbol; }
  inline constexpr $::String::View view() const noexcept { return m_runes.view(); }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles printing string interns.
   * @param os                    Output stream.
   * @param self                  Intern instance.
   */
  static inline void m_print(std::ostream &os, const Intern &self) { os << self.m_runes; }
};

} // namespace Sabre::String

#endif
