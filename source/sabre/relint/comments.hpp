#ifndef _SABRE_RELINT_COMMENTS_HPP
#define _SABRE_RELINT_COMMENTS_HPP

/// Sabre Includes
#include "sabre/forward/relint.hpp"
#include "sabre/syntax/bounds.hpp"

/// Forward Declarations
$_FWD(Sabre::Relint::Prefix, static inline constexpr $::String::View COMMON = "//")
$_FWD(Sabre::Relint::Prefix, static inline constexpr $::String::View MODULE = "///")
$_FWD(Sabre::Relint::Prefix, static inline constexpr $::String::View DOCUMENTS = "//!")

namespace Sabre::Relint {

/// @brief Resolves Bounded Comments.
class Comments {
  //  PROPERTIES  //

  /// @brief Requests getting documentation comments.
  $::String::View m_prefix = Prefix::DOCUMENTS;

  /// @brief The cached comments for a node.
  std::vector<$::String::View> m_leading = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a set of comments.
   * @param mirror                  Mirror to bind.
   * @param prefix                  Comment prefix.
   */
  constexpr Comments(const Mirror *mirror = nullptr, const $::String::View &prefix = Prefix::DOCUMENTS) :
      m_prefix(prefix), m_leading(m_read(mirror)) {}

  //  OPERATOR METHODS  //

  /// @brief Allow conversion for truthiness.
  inline constexpr operator bool() const noexcept { return m_leading.size(); }

  //  PUBLIC METHODS  //

  /// @brief Denotes if there are no comments.
  inline constexpr bool empty() const noexcept { return m_leading.empty(); }

  /// @brief Gets documentation comments only.
  inline constexpr bool documents() const noexcept { return m_prefix == Prefix::DOCUMENTS; }

  /// @brief The expected leading prefix for these comments.
  inline constexpr $::String::View prefix() const noexcept { return m_prefix; }

  /// @brief Gets a view of the comment lines.
  inline constexpr std::span<const $::String::View> span() const noexcept { return m_leading; }

  /// @brief Joins all the lines into one value.
  inline constexpr $::String::Buffer join() const noexcept { return fmt::format("{0}", fmt::join(m_leading, "\n")); }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles reading comments.
   * @param mirror                  Mirror to read.
   */
  std::vector<$::String::View> m_read(const Mirror *mirror) const;

  /**
   * @brief Handles trimming tokens.
   * @param token                   Token to trim.
   */
  $::String::View m_trim(const Lexer::Token &token) const;

  /**
   * @brief Skips all leading tokens (eg: usually declaration values)
   * @param token                   Token to validate.
   * @param outer                   Outer token value.
   */
  bool m_skip(const Lexer::Token *&token, const Lexer::Token *outer) const;

  /**
   * @brief Checks for valid comments.
   * @param token                   Token to validate.
   * @param outer                   Outer token value.
   * @param flag                    Flags for validation.
   */
  bool m_validate(const Lexer::Token *token, const Lexer::Token *outer, bool flag) const;
};

} // namespace Sabre::Relint

#endif
