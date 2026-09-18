#ifndef _SABRE_LEXER_SCANNER_HPP
#define _SABRE_LEXER_SCANNER_HPP

/// Sabre Includes
#include "sabre/diagnostic/reporter.hpp"
#include "sabre/document/buffer.hpp"
#include "sabre/lexer/buffer.hpp"
#include "sabre/lexer/options.hpp"

namespace Sabre::Lexer {

/// @brief Lexical Scanner.
class Scanner {
  //  PROPERTIES  //

  size_t m_line = 1;   // Current line.
  size_t m_column = 1; // Current column.

  size_t m_window = 0;   // Lexeme window offset.
  size_t m_position = 0; // Source buffer position.

  /// @brief The underlying encoding to inherit.
  Encoding m_encoding = Encoding::UTF8;

  /// @brief Document source buffer view.
  $::String::View m_buffer = {};

  /// @brief Diagnostic reporter.
  Diagnostic::Reporter *m_reporter = nullptr;

  /// @brief Current starting position.
  XLSP::Position m_start = XLSP::Position();

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs an empty scanner.
  explicit Scanner(const Options &options = {}) : Scanner("", options) {}

  /**
   * @brief Constructs a lexical scanner.
   * @param buffer                    Source document buffer.
   * @param options                   Lexical options to bind.
   */
  explicit Scanner(const $::String::View &buffer, const Options &options = {})
      : m_encoding(options.encoding), m_buffer(buffer), m_reporter(options.reporter) {}

  //  PUBLIC METHODS  //

  /// @brief Denotes when at the end-of-stream position.
  inline bool eos() const { return m_position >= m_buffer.size(); }

  /// @brief Gets the current head of the stream.
  inline int64_t head() const { return eos() ? EOF : static_cast<uint8_t>(m_buffer[m_position]); }

  /// @brief Gets the current lexeme window.
  inline $::String::View buffer() const { return {&m_buffer[m_window], m_position - m_window}; }

  /// @brief Gets the current token-range.
  inline XLSP::Range range() const { return {m_start, XLSP::Position(m_line, m_column)}; }

  /// @brief Handles skipping over whitespace.
  inline void skip() {
    for (size_t length; $::Encoding::ASCII::hws(read(length));) jump(length);
    m_window = m_position, m_start = XLSP::Position(m_line, m_column); // set
  }

  /**
   * @brief Moves the scanner a set distance.
   * @param length                    Length to jump.
   */
  inline void jump(size_t length) { m_position += length, m_column += m_decode(length); }

  /**
   * @brief Handles reading the next codepoint.
   * @param length                    Read length.
   */
  inline int64_t read(size_t &length) const {
    // stop if we are at our limit
    if (eos()) return EOF;

    const char *original = &m_buffer[m_position]; // get positions now
    const char *head = original, *end = m_buffer.data() + m_buffer.size();

    uint32_t cp; // placeholder output for the codepoint
    $_EXPECT($::Encoding::UTF8::peek(head, end, cp), "Failed to read UTF-8 codepoint");

    // update our positioning as necessary now
    return length = head - original, cp;
  }

  /// @brief Peeks the next available character.
  inline int64_t peek() const {
    size_t length;
    return read(length);
  }

  /// @brief Handles advancing the stream by one codepoint.
  inline int64_t advance() {
    size_t length;
    auto cp = read(length);
    return jump(length), cp;
  }

  /**
   * @brief Attempts matching a lexical character.
   * @param expecting             Character to expect.
   */
  inline bool match(int64_t expecting) { return m_compare(true, expecting); }

  /**
   * @brief Attempts not matching a lexical character.
   * @param expecting             Character to expect.
   */
  inline bool none(int64_t expecting) { return m_compare(false, expecting); }

  /**
   * @brief Checks multiple characters.
   * @tparam As                   Character types.
   * @param initial               Initial character.
   * @param rest                  Addendum characters.
   */
  template <typename... As> inline bool match(int64_t initial, As &&...rest) {
    return m_compare(true, initial, std::forward<As>(rest)...);
  }

  /**
   * @brief Checks not multiple characters.
   * @tparam As                   Character types.
   * @param initial               Initial character.
   * @param rest                  Addendum characters.
   */
  template <typename... As> inline bool none(int64_t initial, As &&...rest) {
    return m_compare(false, initial, std::forward<As>(rest)...);
  }

  /// @brief Updates the scanner with a new-line.
  inline void newline() { $_ASSERT(peek() == '\n'), advance(), ++m_line, m_column = 1; }

  /**
   * @brief Constructs a simple token.
   * @param kind                  Token kind.
   */
  inline Token token(const Kind &kind) const { return token(kind, buffer()); }

  /**
   * @brief Constructs a token with modified buffer.
   * @param kind                  Token kind.
   * @param buffer                Buffer to use.
   */
  inline Token token(const Kind &kind, const $::String::View &buffer) const { return Token(kind, buffer, range()); }

  /**
   * @brief Handles reporting errors.
   * @tparam As                       Argument types.
   * @param code                      Diagnostic code.
   * @param args                      Message arguments.
   */
  template <typename... As> inline void report(Diagnostic::Code code, As &&...args) {
    if (m_reporter) m_reporter->emit(range(), code, std::forward<As>(args)...);
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles decoding diagnostic column lengths.
   * @param length                Length to inherit.
   */
  inline size_t m_decode(size_t length) {
    // UTF-32 encodings should be directly the length given
    if (m_encoding == XLSP::Encoding::Type::UTF32) return length;

    // always decode as a singular value if not UTF-16, or small lengths
    if (m_encoding != XLSP::Encoding::Type::UTF16 || length < 3) return 1;

    // if our value is above 3-bytes, then it will always be a size of 2
    if (length > 3) return 2;

    // we need to decode the codepoint here now
    const char *encoded = &m_buffer[m_position];

    // attempt constructing our-codepoint now as necessary
    return ((*encoded & 0xF) << 12 | (*(encoded + 1) & 0x3F) << 6 | (*(encoded + 2) & 0x3F)) > 0x10000 ? 2 : 1;
  }

  /**
   * @brief Attempts matching a lexical character.
   * @param result                Expected result.
   * @param expecting             Character to expect.
   */
  inline bool m_compare(bool result, int64_t expecting) {
    size_t length; // prepare the length to advance by now
    auto valid = (read(length) == expecting) == result;
    return valid ? (jump(length), true) : false; // compare
  }

  /**
   * @brief Checks multiple characters.
   * @tparam As                   Character types.
   * @param result                Expected result.
   * @param initial               Initial character.
   * @param rest                  Addendum characters.
   */
  template <typename... As> inline bool m_compare(bool result, int64_t initial, As &&...rest) {
    size_t length; // base length
    auto cp = read(length);

    // attempt matching against any inside the array
    auto valid = (cp == initial || (... || (cp == rest))) == result;
    return valid ? (jump(length), true) : false; // compare
  }
};

} // namespace Sabre::Lexer

#endif
