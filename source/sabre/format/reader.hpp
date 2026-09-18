#ifndef _SABRE_FORMAT_READER_HPP
#define _SABRE_FORMAT_READER_HPP

/// Sabre Includes
#include "sabre/format/comments.hpp"
#include "sabre/format/options.hpp"
#include "sabre/format/storage.hpp"
#include "sabre/lexer/visitor.hpp"

namespace Sabre::Format {

/// @brief Format Node Reader.
class Reader : public Lexer::Visitor {
  //  PROPERTIES  //

  /// @brief Available format options.
  const Options *m_options;

  /// @brief Format pieces storage.
  Storage m_storage = {};

  /// @brief Constructs a comments container.
  Comments m_comments = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Do not allow default construction.
  explicit Reader() = delete;

  /**
   * @brief Constructs a format reader.
   * @param tokens                  Lexical tokens.
   * @param options                 Format options.
   */
  explicit Reader(const Lexer::Buffer *tokens);
  explicit Reader(const Lexer::Buffer *tokens, const Options *options);

  //  PUBLIC METHODS  //

  /// @brief Gets the underlying pieces storage.
  inline constexpr Storage *storage() noexcept { return &m_storage; }
  inline constexpr const Storage *storage() const noexcept { return &m_storage; }

  /// @brief Gets the underlying formatter options.
  inline constexpr const Options *options() const noexcept { return m_options; }

  /// @brief Handles skipping leading whitespace.
  inline constexpr bool skip() {
    return m_until([](const Lexer::Token *token) { return !token->flags().test(Lexer::Flag::WHITESPACE); });
  }

  /// @brief Skips until the next token.
  inline constexpr const Lexer::Token *advance() noexcept {
    // stop handling if at the end-of-stream
    if (eos()) return &s_invalid;

    // check that we have a valid token
    $_ASSERT(current()->kind() != Lexer::Kind::MISC_CMT);

    // and update our current token details now
    return ++m_index, m_previous = &m_tokens.at(m_index - 1);
  }

  /// @brief Peeks the next non-whitespace token.
  inline constexpr const Lexer::Token *peek() const noexcept {
    // cache the original details now
    auto offset = m_index;

    // attempt finding the next non-whitespace token
    while (offset < m_tokens.size() && m_tokens.at(offset).kind() == Lexer::Kind::MISC_CMT) ++offset;

    // and return the resulting offset to be used
    return offset >= m_tokens.size() ? &s_invalid : &m_tokens.at(offset);
  }

  /**
   * @brief Flushes the currently queued comments.
   * @param trim                      Force trimming.
   */
  inline constexpr Buffer flush(bool trim = false) {
    return $::Ranges::To<Piece *>(std::views::transform(m_comments.flush(trim), [&](const $::String::View &comment) {
      return comment.empty() ? m_storage.empty() : m_storage.comment(comment);
    }));
  }

  /// @brief Denotes if we have a trailing comment
  inline constexpr Piece *trailing() noexcept {
    // get the previous and current token values now
    const Lexer::Token *before = previous(), *after = current();

    // stop if we fail certain conditions
    if (before->kind() == Lexer::Kind::MISC_CMT) return nullptr;
    if (after->kind() != Lexer::Kind::MISC_CMT) return nullptr;

    // since a valid trailing comment, we want to eat (even if empty)
    auto comment = (++m_index, (m_previous = after)->lexeme());

    // we trail with a line when empty, or a space when a comment exists
    return comment.size() ? m_storage.comment(comment) : nullptr;
  }

  /**
   * @brief Checks for an incoming token.
   * @param kinds                 Token to expect.
   */
  template <std::same_as<Lexer::Kind>... Ks> inline constexpr bool check(const Ks &...kinds) {
    return m_until([kinds...](const Lexer::Token *token) { return ((kinds == token->kind()) || ...); });
  }

  /**
   * @brief Checks for an incoming token.
   * @param flags                 Flag to expect.
   */
  template <std::same_as<Lexer::Flag>... Fs> inline constexpr bool check(const Fs &...flags) {
    return m_until([flags...](const Lexer::Token *token) { return token->flags().test(flags...); });
  }

  /**
   * @brief Checks for an incoming token.
   * @param kinds                 Token to expect.
   */
  template <std::same_as<Lexer::Kind>... Ks> inline constexpr bool match(const Ks &...kinds) {
    return check(kinds...) ? advance(), true : false;
  }

  /**
   * @brief Checks for an incoming token.
   * @param flags                 Flag to expect.
   */
  template <std::same_as<Lexer::Flag>... Fs> inline constexpr bool match(const Fs &...flags) {
    return check(flags...) ? advance(), true : false;
  }

  /**
   * @brief Handles bailing when reading fails.
   * @param args                    Bailout arguments.
   */
  template <class... As> inline constexpr std::nullptr_t bail(As &&...) { return nullptr; }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Attempts skipping to a desired token.
   * @param condition             Condition to skip.
   */
  template <class C> inline constexpr bool m_until(C condition) {
    // cache the original details now
    auto offset = m_index;

    // prepare the pending comments to be appended
    auto pending = Comments::View();

    // attempt iterating over the current token now
    for (auto *token = current(); !condition(token); ++m_index, token = current()) {
      // stop early if we did not receive a comment here
      if (!token->flags().test(Lexer::Flag::WHITESPACE)) return m_rewind(offset), false;

      // otherwise push the comment onto the incoming ones (ignore terminators)
      if (token->kind() == Lexer::Kind::MISC_CMT) pending.emplace_back(token->lexeme());
    }

    // validate all the pending comments to be appended
    return m_comments.append(pending), true;
  }
};

} // namespace Sabre::Format

#endif
