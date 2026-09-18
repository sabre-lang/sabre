#ifndef _SABRE_PARSER_STREAM_HPP
#define _SABRE_PARSER_STREAM_HPP

/// Sabre Includes
#include "sabre/diagnostic/reporter.hpp"
#include "sabre/parser/snapshot.hpp"
#include "sabre/syntax/tree.hpp"

namespace Sabre::Parser {

/// @brief Syntax Parser Stream.
class Stream : public Lexer::Visitor {
  //  PROPERTIES  //

  /// @brief Denotes if a synchronizable error occured.
  bool m_panicking = false;

  /// @brief The outgoing syntax buffer.
  Syntax::Tree *m_syntax = nullptr;

  /// @brief Diagnostics emitter instance.
  Diagnostic::Reporter *m_reporter = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a syntax parser.
   * @param tokens                  Tokens buffer.
   * @param syntax                  Syntax tree instance.
   * @param reporter                Diagnostic reporter.
   */
  explicit Stream(const Lexer::Buffer *tokens, Syntax::Tree *syntax, Diagnostic::Reporter *reporter = nullptr) :
      Visitor(tokens), m_syntax(syntax), m_reporter(reporter) {
    m_skip(); // skip any leading whitespace/comment tokens now
  }

  //  PUBLIC METHODS  //

  /// @brief Denotes if the stream is currently panicking.
  inline constexpr bool panicking() const noexcept { return m_panicking; }

  /// @brief Gets a snapshot of the stream.
  inline constexpr Snapshot snapshot() const noexcept { return Snapshot(this); }

  /// @brief Handles advancing the token stream.
  inline constexpr const Lexer::Token *advance() noexcept {
    if (eos()) return &s_invalid; // ignore end-of-stream
    m_previous = current();       // cache previous value
    ++m_index, m_skip();          // skip over any whitespace
    return m_previous;            // and return previous token
  }

  /// @brief Attempts peeking the next available token.
  inline constexpr const Lexer::Token *peek() const {
    // scan whilst possible to find a suitable token now
    for (size_t offset = m_index + 1; offset < m_tokens.size(); ++offset) {
      auto *token = &m_tokens.at(offset); // get current token
      if (token->kind() != Lexer::Kind::MISC_CMT) return token;
    }

    // failed to find a suitable token
    return &s_invalid;
  }

  /// @brief Checks the current token against various kinds.
  template <std::same_as<Lexer::Kind>... As> inline bool check(const As &...kinds) const {
    return ((kinds == current()->kind()) || ...);
  }

  /// @brief Checks the current token against various flags.
  template <std::same_as<Lexer::Flag>... Fs> inline bool check(const Fs &...flags) const {
    return current()->flags().test(flags...);
  }

  /// @brief Attempts matching a token and advances the visitor if it does.
  template <std::same_as<Lexer::Kind>... As> inline bool match(const As &...kinds) {
    return check(kinds...) ? advance(), true : false;
  }

  /// @brief Attempts matching a token and advances the visitor if it does.
  template <std::same_as<Lexer::Flag>... Fs> inline bool match(const Fs &...flags) {
    return check(flags...) ? advance(), true : false;
  }

  /**
   * @brief Consumes a valid current token.
   * @param kind                  Kind to consume.
   * @param code                  Diagnostic code.
   * @param args                  Format arguments.
   */
  template <class... As> inline bool consume(Lexer::Kind kind, Diagnostic::Code code, As &&...args) {
    return $_LIKELY(match(kind)) ? true : (report(code, std::forward<As>(args)...), false);
  }

  /**
   * @brief Forces a token to be expected.
   * @param kind                  Kind of token.
   * @param args                  Format arguments.
   */
  template <class... As> inline bool expect(Lexer::Kind kind, As &&...args) {
    // stop early if necessary to do so
    if ($_LIKELY(match(kind))) return true;

    // prepare the quoted symbol to be used
    auto symbol = Lexer::Inspect::symbol(kind);
    auto quoted = fmt::format("'{0}'", symbol);

    // and return the resulting failure that occured
    return report(sizeof...(As) ? 2000101 : 2000100, quoted, std::forward<As>(args)...), false;
  }

  /// @brief Handles forcing the parser into a suitable state after panicking.
  inline void synchronize() {
    // declare as not currently panicking
    m_panicking = false;

    // attempt advancing the instance now
    for (; !eos(); advance()) {
      if (match(Lexer::Kind::PUNC_TERM)) break; // terminators or syncable
      else if (current()->flags().test(Lexer::Flag::SYNCABLE)) break;
    }
  }

  /**
   * @brief Handles allocating a syntax-node.
   * @param args                  Node arguments.
   */
  template <std::derived_from<Syntax::Node> T, class... As> inline constexpr T *allocate(As &&...args) {
    return m_syntax->allocate<T>(std::forward<As>(args)...);
  }

  /**
   * @brief Handles reporting errors.
   * @param code                  Diagnostic code.
   * @param args                  Message arguments.
   */
  template <class... As> inline std::nullptr_t report(Diagnostic::Code code, As &&...args) {
    return report(eos() ? previous() : current(), code, std::forward<As>(args)...);
  }

  /**
   * @brief Handles reporting errors.
   * @param token                 Token of error.
   * @param code                  Diagnostic code.
   * @param args                  Message arguments.
   */
  template <class... As> inline std::nullptr_t report(const Lexer::Token *token, Diagnostic::Code code, As &&...args) {
    return $_LIKELY(token) ? report(token->range(), code, std::forward<As>(args)...) : nullptr;
  }

  /**
   * @brief Handles reporting errors.
   * @param node                  Node of error.
   * @param code                  Diagnostic code.
   * @param args                  Message arguments.
   */
  template <class... As> inline std::nullptr_t report(const Syntax::Node *node, Diagnostic::Code code, As &&...args) {
    return $_LIKELY(node) ? report(node->trivia()->range(), code, std::forward<As>(args)...) : nullptr;
  }

  /**
   * @brief Handles reporting errors.
   * @param range                   Source range.
   * @param code                    Diagnostic code.
   * @param args                    Message arguments.
   */
  template <class... As> inline std::nullptr_t report(const XLSP::Range &range, Diagnostic::Code code, As &&...args) {
    // ignore if currently panicking
    if (m_panicking) return nullptr;

    if (m_reporter) m_panicking = m_reporter->emit(range, code, std::forward<As>(args)...);
    else m_panicking = Diagnostic::Inspect::severity(code) == Diagnostic::Severity::ERROR;

    // and return an empty storage value for use
    return nullptr;
  }

private:
  //  PRIVATE METHODS  //

  /// @brief Handles skipping any current whitespace.
  inline constexpr void m_skip() { for (; !eos() && m_current()->kind() == Lexer::Kind::MISC_CMT; ++m_index); }
};

} // namespace Sabre::Parser

#endif
