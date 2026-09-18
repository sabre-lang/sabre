#ifndef _SABRE_FORMAT_DELIMITED_HPP
#define _SABRE_FORMAT_DELIMITED_HPP

/// Sabre Includes
#include "sabre/format/reader.hpp"

namespace Sabre::Format {

/// @brief Dispatching Callback Typing.
using Callback = $::Shared::Functor<Piece *(Reader *)>;

/// @brief Delimited Initializer.
class Delimited {
  //  TYPEDEFS  //

  /// @brief Exit condition typing.
  using Condition = $::Unique::Functor<bool(Reader *)>;

public:
  //  PROPERTIES  //

  /// @brief Denotes if trailing separators are valid.
  bool trailing = false;

  Piece *separator;    // Separator value.
  Callback callback;   // Callback value.
  Condition condition; // Condition value.

  //  CONSTRUCTORS  //

  /// @brief Constructs a delimited set options.
  constexpr Delimited() : Delimited(m_condition) {}

  /**
   * @brief Constructs delimited options.
   * @param separator             Separator to bind.
   * @param condition             Condition to bind.
   */
  constexpr Delimited(Lexer::Kind condition) : Delimited(nullptr, m_callback, condition) {}
  constexpr Delimited(Condition &&condition) : Delimited(nullptr, m_callback, std::move(condition)) {}

  /**
   * @brief Constructs delimited options.
   * @param separator             Separator to bind.
   * @param condition             Condition to bind.
   */
  constexpr Delimited(Piece *separator, Lexer::Kind condition) : Delimited(separator, m_callback, condition) {}
  constexpr Delimited(Piece *separator, Condition &&condition)
      : Delimited(separator, m_callback, std::move(condition)) {}

  /**
   * @brief Constructs delimited options.
   * @param callback              Callback to bind.
   * @param condition             Condition to bind.
   */
  constexpr Delimited(Callback &&callback, Lexer::Kind condition)
      : Delimited(nullptr, std::move(callback), condition) {}

  /**
   * @brief Constructs delimited options.
   * @param callback              Callback to bind.
   * @param condition             Condition to bind.
   */
  constexpr Delimited(Callback &&callback, Condition &&condition)
      : Delimited(nullptr, std::move(callback), std::move(condition)) {}

  /**
   * @brief Constructs delimited options.
   * @param separator             Separator to bind.
   * @param callback              Callback to bind.
   * @param condition             Condition to bind.
   */
  constexpr Delimited(Piece *separator, Callback &&callback) : Delimited(separator, std::move(callback), m_condition) {}

  /**
   * @brief Constructs delimited options.
   * @param separator             Separator to bind.
   * @param callback              Callback to bind.
   * @param condition             Condition to bind.
   */
  constexpr Delimited(Piece *separator, Callback &&callback, Condition &&condition)
      : separator(separator), callback(std::move(callback)), condition(std::move(condition)) {}

  /**
   * @brief Constructs delimited options.
   * @param separator             Separator to bind.
   * @param callback              Callback to bind.
   * @param condition             Condition to bind.
   */
  constexpr Delimited(Piece *separator, Callback &&callback, Lexer::Kind condition)
      : Delimited(separator, std::move(callback), m_closing(condition)) {}

  //  PUBLIC METHODS  //

  /**
   * @brief Checks for delimited exit conditions.
   * @param reader                Format reader.
   */
  inline constexpr bool exit(Reader *reader) noexcept { return condition(reader); }

  /**
   * @brief Consumes the next node available.
   * @param reader                Format reader.
   */
  inline constexpr Piece *consume(Reader *reader) noexcept {
    auto *node = callback(reader); // run the initial callback instance now
    if (separator == reader->storage()->comma()) reader->match(Lexer::Kind::PUNC_COMMA);
    return node; // and return the resulting node that was consumed now
  }

private:
  //  PRIVATE METHODS  //

  /// @brief Default condition value.
  static bool m_condition(Reader *reader);

  /// @brief Default callback value.
  static Piece *m_callback(Reader *reader);

  /**
   * @brief Constructs a suitable condition.
   * @param closing               Closing token.
   */
  static Condition m_closing(Lexer::Kind closing);
};

} // namespace Sabre::Format

#endif
