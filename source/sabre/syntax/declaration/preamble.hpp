#ifndef _SABRE_DECLARATION_PREAMBLE_HPP
#define _SABRE_DECLARATION_PREAMBLE_HPP

/// Sabre Includes
#include "sabre/variable/captures.hpp"
#include "sabre/variable/modifiers.hpp"

/// Syntax Includes
#include "sabre/syntax/declaration/attribute.hpp"
#include "sabre/syntax/declaration/decorator.hpp"

namespace Sabre::Syntax {

/// @brief Alias Node Modifiers.
using Modifiers = ::Sabre::Variable::Modifiers;

/// @brief Declaration Node Abstractions.
class $_ABSTRACT Declaration : public Mixin<Declaration> {
  //  PROPERTIES  //

  /// @brief The preamble target.
  $::String::View m_name;

  /// @brief Bound declaration modifiers.
  Modifiers m_modifiers = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a target.
   * @param tag                   Tag to passthrough.
   * @param name                  Target identifier.
   */
  explicit Declaration(const $::String::View &name) : m_name(name) {}

  /**
   * @brief Constructs a target.
   * @param target                Target identifier.
   */
  explicit Declaration(const Lexer::Token *target) : Declaration(target->lexeme()) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the associated declaration target.
  inline constexpr $::String::View name() const noexcept { return m_name; }

  /// @brief Gets the available modifiers for a declaration.
  inline constexpr Modifiers &modifiers() noexcept { return m_modifiers; }
  inline constexpr const Modifiers &modifiers() const noexcept { return m_modifiers; }
};

/// @brief Encapsulates Declaration Preamble.
class $_ABSTRACT Preamble : public Mixin<Preamble, Declaration> {
  //  PROPERTIES  //

  std::vector<Decorator *> m_decorators = {}; // Runtime decorators.
  std::vector<Attribute *> m_attributes = {}; // Compile-time attributes.

public:
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Mixin<Preamble, Declaration>::Mixin;

  //  PUBLIC METHODS  //

  inline constexpr std::vector<Decorator *> &decorators() noexcept { return m_decorators; }
  inline constexpr const std::vector<Decorator *> &decorators() const noexcept { return m_decorators; }

  inline constexpr std::vector<Attribute *> &attributes() noexcept { return m_attributes; }
  inline constexpr const std::vector<Attribute *> &attributes() const noexcept { return m_attributes; }
};

} // namespace Sabre::Syntax

#endif
