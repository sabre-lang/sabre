#ifndef _SABRE_FORMAT_WRITER_HPP
#define _SABRE_FORMAT_WRITER_HPP

/// Sabre Includes
#include "sabre/format/mixin.hpp"
#include "sabre/format/options.hpp"

namespace Sabre::Format {

/// @brief The baseline writer instance.
class Writer {
  //  PROPERTIES  //

  /// @brief Current width of the written line.
  size_t m_width = 0;

  /// @brief Bound format options.
  const Options *m_options;

  /// @brief Current wrapping list.
  Wrap::List m_wrapped = {};

  /// @brief Current indentation levels.
  $::Map::Set<size_t> m_indents = {};

  /// @brief The underlying output buffer.
  std::vector<$::String::Buffer> m_lines = {""};

  /// @brief Gets the last-most line available.
  $::String::Buffer *m_last = &m_lines.back();

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a writer instance.
   * @param options               Format options.
   */
  explicit Writer();
  explicit Writer(const Options *options);

  //  PUBLIC METHODS  //

  inline constexpr size_t &width() noexcept { return m_width; }
  inline constexpr size_t width() const noexcept { return m_width; }

  inline constexpr Wrap::List &wrapped() noexcept { return m_wrapped; }
  inline constexpr const Wrap::List &wrapped() const noexcept { return m_wrapped; }

  /// @brief Gets the size of the written stream.
  inline constexpr size_t lines() const noexcept { return m_lines.size(); }
  inline constexpr bool empty() const noexcept { return lines() == 1 && !m_width; }

  /// @brief Gets the complete buffer to be written.
  inline constexpr $::String::Buffer merge() const noexcept {
    // prepare the string-stream to write the output to
    auto oss = std::stringstream();

    // append all the available lines now
    for (const auto &line : m_lines) oss << $::Trim::trailing(line) << '\n';

    // prepare the final result to be used
    auto result = $::String::Buffer($::Trim::trailing(oss.view()));

    // and append a final newline if needed
    return result + (!result.empty() && m_options->newline_final ? "\n" : "");
  }

  /// @brief Emplaces a singular space onto the buffer.
  inline constexpr void space() noexcept { m_width += 1, (*m_last) += ' '; }

  /**
   * @brief Handles appending raw text.
   * @param value                 Value to append.
   * @param args                  Additional arguments.
   */
  template <class... As> inline constexpr void concat(const $::String::View &value, As &&...args) noexcept {
    m_width += value.size(), (*m_last) += value;
    if constexpr (sizeof...(As) == 0) return;
    else concat(std::forward<As>(args)...);
  }

  /**
   * @brief Denotes if an item exceeds the current width.
   * @param size                  Size to check.
   */
  inline constexpr bool exceeds(size_t size) const noexcept {
    return Math::Saturating::add(m_width, size) > m_options->line_width;
  }

  /**
   * @brief Denotes if an item exceeds the current width.
   * @param piece                   Piece to check.
   * @param force                   Forced width flag.
   */
  inline constexpr bool exceeds(const Piece *piece, bool force = false) const noexcept {
    return exceeds(piece->width(m_wrapped, force));
  }

  /// @brief Indents the writer forward one-level.
  inline constexpr size_t indent() { return *m_indents.emplace(m_lines.size()).first; }

  /**
   * @brief Dedents the writer by one indenation.
   * @param marker                  Marker to dedent.
   */
  inline constexpr void dedent(size_t marker) { m_indents.erase(marker); }

  /**
   * @brief Handles writing a newline.
   * @param indent                  Optional indentation.
   */
  inline constexpr void newline() noexcept { return newline(m_indents.size()); }
  inline constexpr void newline(size_t indent) noexcept {
    auto width = indent * m_options->indent_width; // prepare the indentation to emplace now
    auto indentation = m_options->indent_spaces ? $::String::Buffer(m_options->indent_width, ' ') : "\t";

    // ensure we prepare the indent and newline
    m_width = width, m_last = &m_lines.emplace_back("");

    // handle emplacing each indentation value
    for (size_t ii = 0; ii < indent; ++ii) (*m_last) += indentation;
  }
};

} // namespace Sabre::Format

#endif
