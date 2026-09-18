#ifndef _SABRE_ANNOTATION_PLACEHOLDER_HPP
#define _SABRE_ANNOTATION_PLACEHOLDER_HPP

/// Sabre Includes
#include "sabre/syntax/node.hpp"

namespace Sabre::Syntax {

/// @brief Generic Parameter Node.
class Placeholder : public Mixin<Placeholder, Annotation> {
  //  PROPERTIES  //

  /// @brief Associated placeholder name.
  $::String::View m_name;

  /// @brief Extensible base class.
  Annotation *m_extends = nullptr;

  /// @brief An optional initial typing.
  Annotation *m_fallback = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a template parameter.
   * @param name                      Name of type.
   */
  explicit Placeholder(const $::String::View &name, Annotation *extends = nullptr, Annotation *fallback = nullptr)
      : m_name(name), m_extends(extends), m_fallback(fallback) {}

  /**
   * @brief Constructs a template parameter.
   * @param token                     Parameter token.
   * @param extends                   Extension type.
   * @param fallback                  Fallback type.
   */
  explicit Placeholder(const Lexer::Token *token, Annotation *extends = nullptr, Annotation *fallback = nullptr)
      : Placeholder(token->lexeme(), extends, fallback) {}

  //  PUBLIC METHODS  //

  inline constexpr $::String::View name() const noexcept { return m_name; }
  inline constexpr const Annotation *extends() const noexcept { return m_extends; }
  inline constexpr const Annotation *fallback() const noexcept { return m_fallback; }
};

} // namespace Sabre::Syntax

#endif
