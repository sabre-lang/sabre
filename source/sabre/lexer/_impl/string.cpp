/// Sabre Includes
#include "sabre/lexer/dispatch.hpp"

//  PRIVATE METHODS  //

void Sabre::Lexer::Dispatch::m_string(Scanner &lexer, Buffer &tokens) {
  // pre-eat the immediate value
  lexer.advance();

  // prepare the validator for incoming strings now
  static constexpr auto is_str = [](int64_t cp) { return cp != '"' && cp != '\n' && cp != EOF; };

  // attempt getting string-characters whilst possible
  for (size_t length; is_str(lexer.read(length));) {
    if (!lexer.match('\\')) lexer.jump(length);
    else if (lexer.read(length) == '"') lexer.jump(length);
  }

  // prepare the current lexeme window now (eg: subtract the leading '"' character)
  auto lexeme = lexer.buffer().substr(1);

  // ensure we now have a final token available
  if (!lexer.match('"')) lexer.report(1000101, "'string-literal'");
  else tokens.append(lexer.token(Kind::LTRL_STR, lexeme));

  // and tail to the next item now
  $_MUSTTAIL return Dispatch::next(lexer, tokens);
}
