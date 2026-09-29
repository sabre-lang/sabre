#ifndef _XLSP_PROTOCOL_POSITION_HPP
#define _XLSP_PROTOCOL_POSITION_HPP

/// XLSP Includes
#include "xlsp/protocol/encoding.hpp"

namespace XLSP {

/// @brief Position Structure.
struct Position {
  //  PROPERTIES  //

  uint32_t line = 0;   // Line-number (zero-offset).
  uint32_t column = 0; // Character-number (zero-offset).

  //  CONSTRUCTORS  //

  /// @brief Constructs an empty position (0, 0).
  constexpr Position() = default;

  /**
   * @brief Constructs a position.
   * @param ln                        Line of position.
   * @param col                       Column of position.
   */
  constexpr Position(uint32_t ln, uint32_t col) : line(ln), column(col) {}

  //  OPERATOR METHODS  //

  /// @brief Equality operator for positions.
  inline constexpr auto operator==(const Position &other) const -> bool {
    return line == other.line && column == other.column;
  }

  /// @brief Comparison operator for positions.
  inline constexpr auto operator<=>(const Position &other) const {
    return std::tie(line, column) <=> std::tie(other.line, other.column);
  }

  //  PUBLIC METHODS  //

  /// @brief Gets a client version of a position.
  inline constexpr Position client() const noexcept {
    return *this == Position() ? *this : Position(line - 1, column - 1);
  }

  /// @brief Gets a server version of a position.
  inline constexpr Position server() const noexcept {
    return *this == Position() ? *this : Position(line + 1, column + 1);
  }

  /**
   * @brief Gets a suitable offset of a view.
   * @param view                      Variable to get offset of.
   * @param encoding                  Encoding to determine offset.
   */
  inline constexpr int64_t offset(const $::String::View &view, const Encoding::Type &encoding) const {
    // prepare the placeholder for a start-of-line
    uint32_t som = 0;

    // attempt getting the start-of-line length
    for (uint32_t ii = 0; ii < line; ++ii) {
      auto nl = view.find('\n', som);
      if (nl == $::String::Term) return -1;
      som = nl + 1; // and increment passed the line
    }

    // get the current line details now
    auto end = view.find('\n', som);
    auto ln = view.substr(som, end);

    // attempt measuring now
    auto total = som + Encoding::measure(ln, column, encoding);

    // return the final result now
    return total > view.size() ? -1 : total;
  }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles encoding positions.
   * @param self                      Position instance.
   */
  static inline $::Serde::Object m_encode(const Position &self) {
    return {{"line", self.line}, {"character", self.column}};
  }

  /**
   * @brief Handles decoding positions.
   * @param value                     JSON value to decode.
   */
  static inline Position m_decode(const $::Serde::Value &value) {
    return {value.at<uint32_t>("line", 0), value.at<uint32_t>("character", 0)};
  }

  /**
   * @brief Handles printing positions.
   * @param os                        Output stream.
   * @param self                      Position to print.
   */
  static inline void m_print(std::ostream &os, const Position &self) { os << self.line << ':' << self.column; }
};

} // namespace XLSP

#endif
