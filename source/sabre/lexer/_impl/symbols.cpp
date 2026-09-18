/// Sabre Includes
#include "sabre/lexer/dispatch.hpp"

//  MACROS  //

#define MM_APPLY(C, ...)                                                                   \
  template <> void Sabre::Lexer::Dispatch::m_symbol<C>(Scanner & lexer, Buffer & tokens) { \
    lexer.advance(), tokens.append(__VA_ARGS__(lexer));                                    \
    $_MUSTTAIL return Dispatch::next(lexer, tokens);                                       \
  }

#define MM_CUSTOM(C, L, T, ...)                                                   \
  template <> void Sabre::Lexer::Dispatch::m_symbol<C>(Scanner & L, Buffer & T) { \
    L.advance();                                                                  \
    __VA_ARGS__;                                                                  \
    $_MUSTTAIL return Dispatch::next(L, T);                                       \
  }

//  TYPEDEFS  //

/// @brief Token Kind Alias.
using K = Sabre::Lexer::Kind;

//  PRIVATE METHODS  //

MM_APPLY('~', [](Scanner &l) -> Token { return l.token(K::UNOP_INV); })
MM_APPLY('!', [](Scanner &l) -> Token { return l.token(l.match('=') ? K::BINOP_NE : K::UNOP_NOT); })
MM_APPLY('^', [](Scanner &l) -> Token { return l.token(l.match('=') ? K::ASOP_XOR : K::BINOP_XOR); })
MM_APPLY('%', [](Scanner &l) -> Token { return l.token(l.match('=') ? K::ASOP_MOD : K::BINOP_MOD); })
MM_APPLY('?', [](Scanner &l) -> Token { return l.token(l.match('?') ? K::BINOP_COAL : K::PUNC_QUERY); })

MM_APPLY('|', [](Scanner &l) -> Token {
  if (l.match('=')) return l.token(K::ASOP_BOR);
  else if (!l.match('|')) return l.token(K::BINOP_BOR);
  return l.token(l.match('=') ? K::ASOP_LOR : K::BINOP_LOR);
})

MM_APPLY('&', [](Scanner &l) -> Token {
  if (l.match('=')) return l.token(K::ASOP_BAND);
  else if (!l.match('&')) return l.token(K::BINOP_BAND);
  return l.token(l.match('=') ? K::ASOP_LAND : K::BINOP_LAND);
})

MM_APPLY('<', [](Scanner &l) -> Token {
  if (l.match('=')) return l.token(K::BINOP_LE);
  else if (!l.match('<')) return l.token(K::BINOP_LT);
  return l.token(l.match('=') ? K::ASOP_SHL : K::BINOP_SHL);
})

MM_APPLY('>', [](Scanner &l) -> Token {
  if (l.match('=')) return l.token(K::BINOP_GE);
  else if (!l.match('>')) return l.token(K::BINOP_GT);
  return l.token(l.match('=') ? K::ASOP_SHR : K::BINOP_SHR);
})

MM_APPLY('=', [](Scanner &l) -> Token {
  if (l.match('>')) return l.token(K::ARROW_BOLD);
  return l.token(l.match('=') ? K::BINOP_EQ : K::ASOP_DEF);
})

MM_APPLY('+', [](Scanner &l) -> Token {
  if (l.match('=')) return l.token(K::ASOP_ADD);
  return l.token(l.match('+') ? K::UNOP_INC : K::BINOP_ADD);
})

MM_APPLY('-', [](Scanner &l) -> Token {
  if (l.match('=')) return l.token(K::ASOP_SUB);
  if (l.match('>')) return l.token(K::ARROW_THIN);
  return l.token(l.match('-') ? K::UNOP_DEC : K::BINOP_SUB);
})

MM_APPLY('*', [](Scanner &l) -> Token {
  if (l.match('=')) return l.token(K::ASOP_MUL);
  if (!l.match('*')) return l.token(K::BINOP_MUL);
  return l.token(l.match('=') ? K::ASOP_POW : K::BINOP_POW);
})

MM_APPLY('(', [](Scanner &l) -> Token { return l.token(K::PUNC_LPAREN); })
MM_APPLY(')', [](Scanner &l) -> Token { return l.token(K::PUNC_RPAREN); })
MM_APPLY('{', [](Scanner &l) -> Token { return l.token(K::PUNC_LBRACE); })
MM_APPLY('}', [](Scanner &l) -> Token { return l.token(K::PUNC_RBRACE); })
MM_APPLY('[', [](Scanner &l) -> Token { return l.token(K::PUNC_LBRACK); })
MM_APPLY(']', [](Scanner &l) -> Token { return l.token(K::PUNC_RBRACK); })

MM_APPLY(',', [](Scanner &l) -> Token { return l.token(K::PUNC_COMMA); })
MM_APPLY(';', [](Scanner &l) -> Token { return l.token(K::PUNC_TERM); })
MM_APPLY(':', [](Scanner &l) -> Token { return l.token(l.match(':') ? K::PUNC_POLICY : K::PUNC_COLON); })

MM_APPLY('@', [](Scanner &l) -> Token { return l.token(K::PUNC_DECOR); })
MM_APPLY('#', [](Scanner &l) -> Token { return l.token(K::PUNC_ATTR); })

MM_CUSTOM('.', lexer, tokens, {
  switch (lexer.match('.') + lexer.match('.')) {
  case 0: tokens.append(lexer.token(K::PUNC_PERIOD)); break;
  case 1: lexer.report(1000101, "spread operator"); break;
  default: tokens.append(lexer.token(K::PUNC_SPREAD)); break;
  }
})

MM_APPLY('/', [](Scanner &l) -> Token {
  return l.match('/') ? m_comment(l) : l.token(l.match('=') ? K::ASOP_DIV : K::BINOP_DIV);
})
