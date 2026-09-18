#ifndef _SABRE_PARSER_DISPATCH_HPP
#define _SABRE_PARSER_DISPATCH_HPP

/// Sabre Includes
#include "sabre/parser/stream.hpp"
#include "sabre/reflect/category.hpp"
#include "sabre/syntax/precedence.hpp"

namespace Sabre::Parser {

/// @brief Parsing Extent.
enum class Extent : uint8_t {
  CLASS,   // class properties
  MODULE,  // module scoping
  SCOPING, // function scoping
};

/// @brief Syntax Dispatch Methods.
class Dispatch : private $::Ensure::Static {
  //  TYPEDEFS  //

  /// @brief The internal expression generator.
  friend struct Syntax::Generator;

  /// @brief Alias the parser precedence.
  using Precedence = Syntax::Precedence;

public:
  //  PUBLIC METHODS  //

  /**
   * @brief Handles launching the parser.
   * @param parser                    Parser stream.
   * @param nodes                     Statement nodes.
   */
  static inline constexpr void stream(Stream *parser, std::vector<Syntax::Node *> &nodes) {
    while (!parser->eos()) {
      auto *node = m_declaration(parser, Extent::MODULE);
      if ($_LIKELY(node)) nodes.emplace_back(node);
      if (parser->panicking()) parser->synchronize();
    }
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Parses a type-annotation.
   * @param parser                    Parser stream.
   */
  static Syntax::Annotation *m_annotation(Stream *parser);

  /**
   * @brief Parses a type-annotation.
   * @param parser                    Parser stream.
   */
  template <std::derived_from<Syntax::Annotation> T> static T *m_annotation(Stream *parser);

  /**
   * @brief Handles parsing template constraints.
   * @param parser                    Parser stream.
   */
  static std::optional<Syntax::Template> m_template(Stream *parser);

  /**
   * @brief Handles parsing function signatures/prototypes.
   * @param parser                    Parser stream.
   * @param annotated                 Annotation flag.
   */
  static Syntax::Signature *m_signature(Stream *parser, bool annotated = false);
  static Syntax::Parameters *m_parameters(Stream *parser, bool annotated = false);
  static Syntax::Constructor *m_constructor(Stream *parser, bool annotated = false);

  /**
   * @brief Handles constructing anonymous qualifiers.
   * @param parser                    Parser stream.
   * @param name                      Name of qualifier.
   */
  static Syntax::Annotation *m_qualifier(Stream *parser, const $::String::View &name);

  /**
   * @brief Parses a top-level declaration node.
   * @param parser                    Parser stream.
   * @param extent                    Scoping extent.
   */
  static Syntax::Node *m_declaration(Stream *parser, Extent extent);

  /**
   * @brief Parses a top-level declaration node.
   * @param parser                    Parser stream.
   */
  template <std::derived_from<Syntax::Node> T> static T *m_declaration(Stream *parser);

  /**
   * @brief Parses a declaration subject with details.
   * @param parser                    Syntax parser.
   * @param extent                    Scoping extent.
   */
  static Syntax::Declaration *m_subject(Stream *parser, Extent extent);
  static Syntax::Declaration *m_preamble(Stream *parser, Extent extent);
  static Syntax::Declaration *m_modifiers(Stream *parser, Extent extent);

  /**
   * @brief Handles parsing attributes/decorators.
   * @param parser                    Syntax parser.
   */
  static Reflect::Category m_category(Stream *parser);
  static Syntax::Attribute *m_attribute(Stream *parser);
  static Syntax::Decorator *m_decorator(Stream *parser);

  /**
   * @brief Handles parsing any import/export statement.
   * @param parser                    Syntax parser.
   * @param extent                    Scoping extent.
   */
  static Syntax::Node *m_import(Stream *parser, Extent extent);
  static Syntax::Node *m_export(Stream *parser, Extent extent);

  /**
   * @brief Parses any variable declaration.
   * @param parser                    Syntax parser.
   * @param annotated                 Annotation flag.
   */
  static Syntax::Variable *m_variable(Stream *parser, bool annotated = false);

  /**
   * @brief Handles parsing super-classes.
   * @param parser                    Syntax parser.
   */
  static Syntax::Expression *m_extends(Stream *parser);

  /**
   * @brief Handles parsing implementation interfaces.
   * @param parser                    Syntax parser.
   */
  static Syntax::Specialization m_implements(Stream *parser);

  /**
   * @brief Parses a statement node.
   * @param parser                    Parser stream.
   */
  static Syntax::Node *m_statement(Stream *parser);
  static Syntax::Node *m_statement(Stream *parser, Syntax::Unterminated);

  /**
   * @brief Parses a statement node.
   * @param parser                    Parser stream.
   */
  template <std::derived_from<Syntax::Node> T> static T *m_statement(Stream *parser);

  /**
   * @brief Handles parsing code-blocks.
   * @param parser                    Syntax parser.
   * @param module                    Top-level module flag.
   */
  static Syntax::Block *m_block(Stream *parser, Extent extent);

  /**
   * @brief Parses an expression with default precedence.
   * @param parser                    Syntax parser.
   * @param rank                      Precedence rank.
   */
  static Syntax::Expression *m_expression(Stream *parser, Precedence rank = Precedence::ASSIGN);

  /**
   * @brief Handles parsing names and identifiers.
   * @param parser                    Syntax parser.
   */
  static const Lexer::Token *m_label(Stream *parser);
  static Syntax::Identifier *m_identifier(Stream *parser);
  static Syntax::Identifier *m_identifier(Stream *parser, const $::String::View &name);

  /**
   * @brief Handles parsing match query guards.
   * @param parser                    Syntax parser.
   * @param node                      Node to validate.
   */
  static Syntax::Expression *m_guard(Stream *parser);

  /**
   * @brief Parses a prefix expression.
   * @param parser                    Syntax parser.
   * @param assignable                Assignability flag.
   */
  template <std::derived_from<Syntax::Expression> T>
  static Syntax::Expression *m_prefix(Stream *parser, bool assignable);

  /**
   * @brief Parses an infix expression.
   * @param parser                    Syntax parser.
   * @param lvalue                    LHS value.
   * @param assignable                Assignability flag.
   */
  template <std::derived_from<Syntax::Expression> T>
  static Syntax::Expression *m_infix(Stream *parser, Syntax::Expression *lvalue, bool assignable);

  /**
   * @brief Handles terminating nodes with ';'.
   * @param parser                    Syntax parser.
   * @param node                      Node to terminate.
   * @param describe                  After description.
   */
  static Syntax::Node *m_terminates(Stream *parser, Syntax::Node *node, const $::String::View &describe);

  /**
   * @brief Handles terminating declarations with ';'.
   * @param parser                    Syntax parser.
   * @param describe                  After description.
   */
  template <std::derived_from<Syntax::Declaration> T>
  static inline Syntax::Node *m_terminates(Stream *parser, const $::String::View &describe) {
    return m_terminates(parser, m_declaration<T>(parser), describe);
  }

  /**
   * @brief Handles terminating statements with ';'.
   * @param parser                    Syntax parser.
   * @param describe                  After description.
   */
  template <std::derived_from<Syntax::Statement> T>
  static inline Syntax::Node *m_terminates(Stream *parser, const $::String::View &describe) {
    return m_terminates(parser, m_statement<T>(parser), describe);
  }

  /**
   * @brief Forces a debug assertion for certain tokens.
   * @param token                     Token received.
   * @param expected                  Expected token.
   */
  static inline const Lexer::Token *m_assert(const Lexer::Token *token, $_UNUSED Lexer::Kind expected) {
    return $_ASSERT(token->kind() == expected), token;
  }

  /**
   * @brief Forces a debug assertion for certain tokens.
   * @param token                     Token received.
   * @param flag                      Flag to be tested.
   */
  static inline const Lexer::Token *m_assert(const Lexer::Token *token, $_UNUSED Lexer::Flag flag) {
    return $_ASSERT(token->flags().test(flag)), token;
  }

  /**
   * @brief Forces a debug assertion for certain tokens.
   * @param token                     Token received.
   * @param lexeme                    Expected lexeme.
   */
  static inline const Lexer::Token *m_assert(const Lexer::Token *token, $_UNUSED const $::String::View &lexeme) {
    return $_ASSERT(token->lexeme() == lexeme), token;
  }
};

} // namespace Sabre::Parser

#endif
