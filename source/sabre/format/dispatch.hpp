#ifndef _SABRE_FORMAT_DISPATCH_HPP
#define _SABRE_FORMAT_DISPATCH_HPP

/// Sabre Includes
#include "sabre/format/collection.hpp"
#include "sabre/format/delimited.hpp"
#include "sabre/syntax/precedence.hpp"
#include "sabre/variable/modifiers.hpp"

namespace Sabre::Format {

/// @brief Handles Reader Dispatching.
class Dispatch : private $::Ensure::Static {
  //  TYPEDEFS  //

  /// @brief Allow delimited lists access.
  friend class Delimited;

  /// @brief Allow the generator internal access.
  friend struct Syntax::Generator;

  /// @brief Dispatch precedence.
  using Precedence = Syntax::Precedence;

public:
  //  PUBLIC METHODS  //

  /**
   * @brief Attempts reading a program of tokens.
   * @param reader                  Format reader.
   */
  static inline Piece *program(Reader *reader) noexcept { return m_collection(reader); }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles reading annotations.
   * @param reader                  Format reader.
   */
  template <std::derived_from<Syntax::Node> T = Syntax::Node> static Piece *m_annotation(Reader *reader);

  /**
   * @brief Handles parsing template parameters.
   * @param reader                  Format reader.
   */
  static Piece *m_template(Reader *reader);

  /**
   * @brief Handles parsing function signatures.
   * @param reader                  Format reader.
   */
  static Piece *m_signature(Reader *reader, bool compressed = false);
  static Piece *m_parameters(Reader *reader, bool compressed = false);
  static Piece *m_constructor(Reader *reader, bool compressed = false);

  /**
   * @brief Handles reading declarations.
   * @param reader                  Format reader.
   */
  template <std::derived_from<Syntax::Node> T = Syntax::Node> static Piece *m_declaration(Reader *reader);

  /**
   * @brief Handles incoming import/exports.
   * @param reader                  Format reader.
   */
  static Piece *m_export(Reader *reader);
  static Piece *m_import(Reader *reader, bool exported);

  /**
   * @brief Handles subject parsing.
   * @param reader                  Format reader.
   * @param kind                    Lexical kind.
   */
  static Piece *m_subject(Reader *reader);
  static Piece *m_subject(Reader *reader, Lexer::Kind kind);

  /**
   * @brief Handles parsing declaration preamble.
   * @param reader                  Format reader.
   */
  static Piece *m_preamble(Reader *reader);

  /**
   * @brief Handles parsing declaration modifiers.
   * @param reader                  Format reader.
   * @param modifiers               Variable modifiers.
   */
  static Piece *m_modifiers(Reader *reader, Variable::Modifiers modifiers = {});

  /**
   * @brief Handles parsing declaration attributes/decorators.
   * @param reader                  Format reader.
   */
  static Piece *m_category(Reader *reader);
  static Piece *m_attribute(Reader *reader);
  static Piece *m_decorator(Reader *reader);

  /**
   * @brief Handles parsing class implementations.
   * @param reader                  Format reader.
   */
  static Piece *m_extends(Reader *reader);
  static Piece *m_implements(Reader *reader);

  /**
   * @brief Handles parsing a collection of nodes.
   * @param reader                  Format reader.
   * @param options                 Options to bind.
   */
  static Piece::List *m_collection(Reader *reader, Delimited &&options = {});
  static Piece::List *m_delimited(Reader *reader, Delimited &&options = {});

  /**
   * @brief Handles reading variables.
   * @param reader                  Format reader.
   * @param compress                Parameter compressed.
   */
  static Piece *m_variable(Reader *reader, bool compressed = false);

  /**
   * @brief Handles reading statements.
   * @param reader                  Format reader.
   * @param group                   Block header.
   */
  template <std::derived_from<Syntax::Node> T = Syntax::Node>
  static Piece *m_statement(Reader *reader, Piece::Group *group = nullptr);

  /**
   * @brief Handles reading unterminated statements.
   * @param reader                  Format reader.
   */
  static Piece *m_statement(Reader *reader, Syntax::Unterminated);

  /**
   * @brief Handles parsing blocks.
   * @param reader                  Format reader.
   * @param group                   Associated group.
   */
  static Piece *m_block(Reader *reader, Piece::Group *group = nullptr);

  /**
   * @brief Handles reading expressions.
   * @param reader                  Format reader.
   * @param rank                    Precendence rank.
   */
  static Piece *m_expression(Reader *reader, Precedence rank = Precedence::ASSIGN);

  /**
   * @brief Handles parsing a condition node.
   * @param reader                  Format reader.
   */
  static Piece *m_conditional(Reader *reader);

  /**
   * @brief Handles parsing binary expressions.
   * @param reader                  Format reader.
   * @param left                    Left-most node.
   * @param right                   Right-most callback.
   */
  static Piece *m_binary(Reader *reader, Piece *left, Callback &&right);

  /**
   * @brief Handles parsing prefixed expressions.
   * @param reader                  Format reader.
   */
  template <std::derived_from<Syntax::Expression> T> static Piece *m_prefix(Reader *reader, bool);

  /**
   * @brief Handles parsing infixed expressions.
   * @param reader                  Format reader.
   * @param value                   Left-most value.
   */
  template <std::derived_from<Syntax::Expression> T> static Piece *m_infix(Reader *reader, Piece *value, bool = false);

  /**
   * @brief Handles assignment parsing.
   * @param reader                  Format reader.
   * @param target                  Target to assign.
   * @param callback                Value callback.
   */
  static Piece *m_assignment(Reader *reader, Piece *target);
  static Piece *m_assignment(Reader *reader, Piece *target, Callback &&callback);

  /**
   * @brief Handles parsing a valid identifier.
   * @param reader                  Format reader.
   */
  static Piece *m_identifier(Reader *reader);

  /**
   * @brief Handles parsing an identifier chain.
   * @param reader                  Format reader.
   */
  static Piece *m_chain(Reader *reader);

  /**
   * @brief Handles specializing a template.
   * @param reader                  Format reader.
   * @param qualifier               Qualifier to specialize.
   */
  static Piece *m_specialize(Reader *reader, Piece *qualifier);

  /**
   * @brief Handles parsing with leading comments.
   * @param reader                  Format reader.
   * @param callback                Incoming node callback.
   * @param trim                    Forcibly trim comments.
   */
  static Buffer m_leading(Reader *reader, bool trim = true);
  static Piece *m_leading(Reader *reader, Callback &&callback, bool trim = true);

  /**
   * @brief Handles parsing with trailing comments.
   * @param reader                  Format reader.
   */
  static Piece *m_trailing(Reader *reader);
  static Piece *m_trailing(Reader *reader, Piece *piece);
  static Piece *m_trailing(Reader *reader, Callback &&callback);

  /**
   * @brief Handles terminating nodes with a semicolon.
   * @param reader                  Format reader.
   * @param piece                   Piece to terminate.
   */
  static Piece *m_terminates(Reader *reader, Piece *piece);

  /**
   * @brief Handles terminating declarations.
   * @param reader                  Format reader.
   */
  template <std::derived_from<Syntax::Declaration> T> static inline Piece *m_terminates(Reader *reader) {
    return m_terminates(reader, m_declaration<T>(reader));
  }

  /**
   * @brief Handles terminating statements.
   * @param reader                  Format reader.
   */
  template <std::derived_from<Syntax::Statement> T> static inline Piece *m_terminates(Reader *reader) {
    return m_terminates(reader, m_statement<T>(reader));
  }
};

} // namespace Sabre::Format

#endif
