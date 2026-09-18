/// Sabre Includes
#include "sabre/lexer/dispatch.hpp"

//  PRIVATE METHODS  //

Sabre::Lexer::Token Sabre::Lexer::Dispatch::m_comment(Scanner &lexer) {
  $_ASSERT(lexer.buffer() == "//", "Invalid comment lexeme");
  while (lexer.none('\n', EOF)); // advance whilst not the EOS/EOL

  // resolve the comment token before any new-line characters
  auto value = $::Trim::trailing(lexer.buffer());
  auto comment = lexer.token(Kind::MISC_CMT, value);

  if (lexer.peek() == '\n') lexer.newline();
  return comment; // return the comment
}

template <> void Sabre::Lexer::Dispatch::m_symbol<'\n'>(Scanner &lexer, Buffer &tokens) {
  tokens.append(lexer.token(Kind::MISC_CMT)), lexer.newline();
  $_MUSTTAIL return Dispatch::next(lexer, tokens); // tail-call
}
