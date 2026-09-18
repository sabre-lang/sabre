/// Sabre Includes
#include "sabre/lexer/dispatch.hpp"

//  PRIVATE METHODS  //

void Sabre::Lexer::Dispatch::m_identifier(Scanner &lexer, Buffer &tokens) {
  // prepare a list of available keywords now
  static $::Map::Record<Kind> keywords = {
#define SABRE_XX_TOKEN_KEYWORD(N, S, ...) {S, Kind::N},
#include "sabre/lexer/_defines/tokens.def"
  };

  // prepare a simple reader to be used
  static constexpr auto is_ident = [](int64_t cp) {
    return $::Encoding::ASCII::ident(cp) || $::Encoding::ASCII::num(cp);
  };

  // attempt eating values whilst possible to do so
  for (size_t length; is_ident(lexer.read(length));) lexer.jump(length);

  auto lexeme = lexer.buffer(); // attempt classifying the buffer now
  auto kind = keywords.contains(lexeme) ? keywords.at(lexeme) : Kind::LTRL_IDENT;
  tokens.append(lexer.token(kind)); // and emplace the associated value

  // and tail to the next item now
  $_MUSTTAIL return Dispatch::next(lexer, tokens);
}
