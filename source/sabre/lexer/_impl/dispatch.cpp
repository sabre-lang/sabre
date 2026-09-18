/// Sabre Includes
#include "sabre/lexer/dispatch.hpp"

/// Lexer Includes
#include "sabre/lexer/_inline/generator.ipp"

//  PUBLIC METHODS  //

void Sabre::Lexer::Dispatch::next(Scanner &lexer, Buffer &buffer) {
  // start by skipping all leading whitespace now
  lexer.skip();

  // read the current head value
  auto head = lexer.head();

  // if we have an eos value, then stop
  if ($_UNLIKELY(head < 0)) return;

  // and tail call this method now for performance
  static constexpr auto s_table = Sabre::Lexer::Generator::table();
  $_MUSTTAIL return s_table[static_cast<uint8_t>(head)](lexer, buffer);
}

//  PRIVATE METHODS  //

$::String::Buffer Sabre::Lexer::Dispatch::m_escape(int64_t cp) {
  // fast-path values that are less-than 0x20
  if (cp < 0x20) return fmt::format("U+{0:2X}", cp);

  // ensure the value is now valid
  $_ASSERT(0 < cp && cp < UINT8_MAX, "Exceeded UTF-8 range");

  // convert to a suitable quoted value now
  return $::Encoding::UTF8::from(static_cast<uint32_t>(cp));
}

void Sabre::Lexer::Dispatch::m_invalid(Scanner &lexer, Buffer &tokens) {
  // advance past this current codepoint
  auto cp = lexer.advance();

  // and report a suitable error now
  lexer.report(1000100, m_escape(cp));

  // and ensure we tail-call to next instruction
  $_MUSTTAIL return next(lexer, tokens);
}
