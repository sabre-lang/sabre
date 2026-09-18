#ifndef _SABRE_STATEMENT_IMPORT_HPP
#define _SABRE_STATEMENT_IMPORT_HPP

/// Syntax Includes
#include "sabre/syntax/literal/identifier.hpp"

namespace Sabre::Syntax {

/// @brief Import Statement Node.
class Import : public Mixin<Import, Statement> {
  //  PROPERTIES  //

  /// @brief Denotes if a module export instead.
  bool m_exported;

  /// @brief Denotes if using wildcard naming.
  bool m_wildcard;

  /// @brief The import path given.
  $::String::View m_path;

  /// @brief Import binding patterns.
  std::vector<Identifier *> m_patterns;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an import statement.
   * @param token                 Resource token.
   * @param exported              Exported flag.
   */
  explicit Import(const Lexer::Token *token, bool exported = false) : Import(token->lexeme(), nullptr, exported) {}

  /**
   * @brief Constructs an import statement.
   * @param path                  Resource path.
   * @param exported              Exported flag.
   */
  explicit Import(const $::String::View &path, bool exported = false) : Import(path, nullptr, exported) {}

  /**
   * @brief Constructs an import statement.
   * @param token                 Resource token.
   * @param pattern               Wildcard pattern.
   * @param exported              Exported flag.
   */
  explicit Import(const Lexer::Token *token, Identifier *pattern, bool exported = false)
      : Import(token->lexeme(), pattern, exported) {}

  /**
   * @brief Constructs an import statement.
   * @param path                  Resource path.
   * @param pattern               Wildcard pattern.
   * @param exported              Exported flag.
   */
  explicit Import(const $::String::View &path, Identifier *pattern, bool exported = false)
      : m_exported(exported), m_wildcard(true), m_path(path) {
    if (pattern != nullptr) m_patterns.emplace_back(pattern);
  }

  /**
   * @brief Constructs an import statement.
   * @param path                  Resource path.
   * @param patterns              Import patterns.
   * @param exported              Exported flag.
   */
  explicit Import(const Lexer::Token *token, std::vector<Identifier *> patterns = {}, bool exported = false)
      : Import(token->lexeme(), patterns, exported) {}

  /**
   * @brief Constructs an import statement.
   * @param path                  Resource path.
   * @param patterns              Import patterns.
   * @param exported              Exported flag.
   */
  explicit Import(const $::String::View &path, std::vector<Identifier *> patterns = {}, bool exported = false)
      : m_exported(exported), m_wildcard(false), m_path(path), m_patterns(patterns) {}

  //  PUBLIC METHODS  //

  inline constexpr bool exported() const noexcept { return m_exported; }
  inline constexpr bool wildcard() const noexcept { return m_wildcard; }
  inline constexpr $::String::View path() const noexcept { return m_path; }
  inline constexpr const std::vector<Identifier *> &patterns() const noexcept { return m_patterns; }
};

} // namespace Sabre::Syntax

#endif
