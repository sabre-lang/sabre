#ifndef _SABRE_DECLARATION_VARIABLE_HPP
#define _SABRE_DECLARATION_VARIABLE_HPP

/// Syntax Includes
#include "sabre/syntax/declaration/preamble.hpp"

namespace Sabre::Syntax {

/// @brief Variable Declaration Node.
struct Variable : public Mixin<Variable, Preamble> {
  //  TYPEDEFS  //

  /// @brief Allow use of variable flags here.
  using Flag = ::Sabre::Variable::Flag;

private:
  //  PROPERTIES  //

  /// @brief Type annotation hint.
  Annotation *m_hint = nullptr;

  /// @brief Associated initial value.
  Expression *m_initializer = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a variable declaration.
   * @param name                      Target name.
   */
  explicit Variable(const $::String::View &name) : Variable(name, nullptr, nullptr) {}

  /**
   * @brief Constructs a variable declaration.
   * @param target                    Target name.
   */
  explicit Variable(const Lexer::Token *target) : Variable(target, nullptr, nullptr) {}

  /**
   * @brief Constructs a variable declaration.
   * @param name                      Target name.
   * @param hint                      Type annotation.
   */
  explicit Variable(const $::String::View &name, Expression *initializer) : Variable(name, nullptr, initializer) {}

  /**
   * @brief Constructs a variable declaration.
   * @param target                    Target name.
   * @param initializer               Initializer value.
   */
  explicit Variable(const Lexer::Token *target, Expression *initializer) : Variable(target, nullptr, initializer) {}

  /**
   * @brief Constructs a variable declaration.
   * @param name                      Target name.
   * @param initializer               Initializer value.
   */
  explicit Variable(const $::String::View &name, Annotation *hint) : Variable(name, hint, nullptr) {}

  /**
   * @brief Constructs a variable declaration.
   * @param target                    Target name.
   * @param hint                      Type annotation.
   */
  explicit Variable(const Lexer::Token *target, Annotation *hint) : Variable(target, hint, nullptr) {}

  /**
   * @brief Constructs a variable declaration.
   * @param target                    Target name.
   * @param hint                      Type annotation.
   * @param initializer               Initializer value.
   */
  explicit Variable(const Lexer::Token *target, Annotation *hint, Expression *initializer)
      : Mixin(target), m_hint(hint), m_initializer(initializer) {}

  /**
   * @brief Constructs a variable declaration.
   * @param name                      Target name.
   * @param hint                      Type annotation.
   * @param initializer               Initializer value.
   */
  explicit Variable(const $::String::View &name, Annotation *hint, Expression *initializer)
      : Mixin(name), m_hint(hint), m_initializer(initializer) {}

  //  PUBLIC METHODS  //

  inline constexpr const Annotation *hint() const noexcept { return m_hint; }
  inline constexpr const Expression *initializer() const noexcept { return m_initializer; }

  inline constexpr bool required() const noexcept { return !optional() && !m_initializer; }
  inline constexpr bool optional() const noexcept { return modifiers().test(Flag::OPTIONAL); }
  inline constexpr bool immutable() const noexcept { return !modifiers().test(Flag::MUTABLE); }
  inline constexpr bool disposable() const noexcept { return modifiers().test(Flag::DISPOSABLE); }
};

} // namespace Sabre::Syntax

#endif
