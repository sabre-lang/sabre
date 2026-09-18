/// Syntax Includes
#include "sabre/syntax/_inline/expression.ipp"

//  PRIVATE METHODS  //

const Sabre::Lexer::Token *Sabre::Parser::Dispatch::m_label(Stream *parser) {
  // attempt matching an annotatable token
  if (parser->match(Lexer::Flag::ANNOTATES)) return parser->previous();

  // report an error here based on the reserved state of the item
  auto reserved = parser->check(Lexer::Flag::RESERVED);
  auto message = reserved ? parser->current()->lexeme() : "an identifier";
  return parser->report(reserved ? 2000102 : 2000100, message);
}

Sabre::Syntax::Identifier *Sabre::Parser::Dispatch::m_identifier(Stream *parser) {
  auto *token = m_label(parser);
  if (token == nullptr) return nullptr;
  return parser->allocate<Syntax::Identifier>(Syntax::Bounds(token), token);
}

Sabre::Syntax::Identifier *Sabre::Parser::Dispatch::m_identifier(Stream *parser, const $::String::View &name) {
  return parser->allocate<Syntax::Identifier>(Syntax::Bounds(), name);
}

SABRE_MM_PARSE_PREFIX(Self, parser, ) {
  auto *token = m_assert(parser->advance(), Lexer::Kind::LTRL_SELF);
  return parser->allocate<Syntax::Self>(Syntax::Bounds(token));
}

SABRE_MM_PARSE_PREFIX(Identifier, parser, assignable) {
  // prepare a potential snapshot to be used
  auto snapshot = parser->snapshot();

  // parse the underlying identifier to be used
  auto *identifier = m_identifier(parser);
  if (identifier == nullptr) return nullptr;

  if (!assignable) return identifier; // ignore not assignable
  if (!parser->match(Lexer::Flag::ASSIGNS)) return identifier;

  // get the incoming assignment operator now
  auto *opcode = parser->previous();

  // get the underlying value now
  auto value = m_expression(parser);
  if (value == nullptr) return nullptr;

  // prepare the bounds to be used
  const auto bounds = snapshot.enclose(opcode);

  // check the type of opcode that we have
  switch (opcode->kind()) {
#define SABRE_XX_TOKEN_ASSIGN(K, ...)                                                            \
  case Lexer::Kind::ASOP_##K:                                                                    \
    value = parser->allocate<Syntax::Binary>(bounds, Lexer::Kind::BINOP_##K, identifier, value); \
    break;

// include all the available assignment tokens now
#include "sabre/lexer/_defines/tokens.def"

  // break for unknown opcodes given
  default: break;
  }

  // and build the resulting assignment expression
  return parser->allocate<Syntax::Assign>(bounds, identifier, value);
}
