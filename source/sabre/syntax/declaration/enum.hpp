#ifndef _SABRE_DECLARATION_ENUM_HPP
#define _SABRE_DECLARATION_ENUM_HPP

/// Syntax Includes
#include "sabre/syntax/declaration/preamble.hpp"

namespace Sabre::Syntax {

/// @brief Enum Variant Node.
class Variant : public Mixin<Variant> {
  //  PROPERTIES  //

  $::String::View m_key; // Variant key.
  Expression *m_label;   // Optional label.
  Expression *m_value;   // Optional value.

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an enum variant.
   * @param token                 Variant token.
   * @param label                 Optional label.
   * @param value                 Optional value.
   */
  explicit Variant(const Lexer::Token *token, Expression *label, Expression *value)
      : Variant(token->lexeme(), label, value) {}

  /**
   * @brief Constructs an enum variant.
   * @param key                   Key to bind.
   * @param label                 Optional label.
   * @param value                 Optional value.
   */
  explicit Variant(const $::String::View &key, Expression *label, Expression *value)
      : m_key(key), m_label(label), m_value(value) {}

  //  PUBLIC METHODS  //

  inline constexpr $::String::View key() const noexcept { return m_key; }
  inline constexpr const Expression *label() const noexcept { return m_label; }
  inline constexpr const Expression *value() const noexcept { return m_value; }
};

/// @brief Enum Declaration Node.
class Enum : public Mixin<Enum, Declaration> {
  //  PROPERTIES  //

  /// @brief All enum variants.
  std::vector<Variant *> m_variants = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an enumeration.
   * @param token             Name of enum.
   * @param variants          Enumeration variants.
   */
  explicit Enum(const Lexer::Token *token, const std::vector<Variant *> &variants = {})
      : Enum(token->lexeme(), variants) {}

  /**
   * @brief Constructs an enumeration.
   * @param name              Name of enum.
   * @param variants          Enumeration variants.
   */
  explicit Enum(const $::String::View &name, const std::vector<Variant *> &variants = {})
      : Mixin(name), m_variants(variants) {}

  //  PUBLIC METHODS  //

  /// @brief Denotes if the enumeration is empty.
  inline constexpr bool empty() const noexcept { return m_variants.empty(); }

  /// @brief Gets the available enumeration variants.
  inline constexpr const std::vector<Variant *> &variants() const noexcept { return m_variants; }
};

} // namespace Sabre::Syntax

#endif
