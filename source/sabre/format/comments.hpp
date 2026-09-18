#ifndef _SABRE_FORMAT_COMMENTS_HPP
#define _SABRE_FORMAT_COMMENTS_HPP

/// Sabre Includes
#include "sabre/forward/format.hpp"
#include "sabre/lexer/token.hpp"

namespace Sabre::Format {

/// @brief Comments Container.
struct Comments {
  //  TYPEDEFS  //

  /// @brief Comments View Available.
  using View = std::vector<$::String::View>;

private:
  //  PROPERTIES  //

  /// @brief Current comments buffer.
  View m_buffer = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs an empty set of comments.
  constexpr Comments() = default;

  //  PUBLIC METHODS  //

  /// @brief Denotes if the comments are empty.
  inline constexpr bool empty() const noexcept { return m_buffer.empty(); }

  /// @brief Gets the current comments size.
  inline constexpr size_t size() const noexcept { return m_buffer.size(); }

  /**
   * @brief Flushes the underlying buffer.
   * @param trim                  Trimming flag.
   */
  inline constexpr View flush(bool trim = false) { return m_compress(trim); }

  /**
   * @brief Handles appending a singular comment.
   * @param comment               Comment to append.
   */
  inline constexpr void append(const $::String::View &comment) { m_buffer.emplace_back(comment); }

  /**
   * @brief Handles appending an incoming buffer.
   * @param buffer                Buffer to append.
   */
  inline constexpr void append(const View &buffer) {
    m_rtrim(m_buffer), std::ranges::for_each(buffer, [&](const $::String::View &comment) { append(comment); });
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles compressing the underlying buffer instance.
   * @param trim                          Trims leading whitespace.
   */
  inline constexpr View m_compress(bool trim) {
    // prepare an output buffer to be written to
    auto output = View();

    // iterate over the underlying buffer (with move)
    for (const auto &comment : m_buffer) {
      if (m_expand(output, comment, trim)) output.emplace_back(comment);
    }

    // post-trim our buffer if necessary now
    if (trim) m_rtrim(output);

    // return the resulting compressed output
    return m_buffer = View(), output;
  }

  /**
   * @brief Handles trimming the right-side of a buffer.
   * @param buffer                        Buffer to trim-right.
   */
  inline constexpr void m_rtrim(View &buffer) const {
    while (buffer.size() && buffer.back().empty()) buffer.pop_back();
  }

  /**
   * @brief Checks if we can emplace a comment onto a buffer.
   * @param buffer                        Buffer to append to.
   * @param comment                       Comment to be appended.
   * @param trim                          The current trim state.
   */
  inline constexpr bool m_expand(const View &buffer, const $::String::View &comment, bool trim) {
    // always append if the comment is not empty
    if (comment.size()) return true;

    // if the buffer is empty, validate based on trim state
    if (buffer.empty()) return !trim;

    // otherwise only expand when the backmost item is not empty
    return !buffer.back().empty();
  }
};

} // namespace Sabre::Format

#endif
