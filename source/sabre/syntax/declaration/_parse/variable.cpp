/// Syntax Includes
#include "sabre/syntax/_inline/declaration.ipp"

//  PRIVATE METHODS  //

Sabre::Syntax::Variable *Sabre::Parser::Dispatch::m_variable(Stream *parser, bool annotated) {
  // prepare the incoming snapshot
  auto snapshot = parser->snapshot();

  // parse the incoming immutability flag if found
  bool immutable = parser->match(Lexer::Kind::DECL_LET, Lexer::Kind::DECL_USE);
  bool disposable = immutable && parser->previous()->kind() == Lexer::Kind::DECL_USE;

  // handle various details surrounding immutibility
  if (!immutable) immutable = !parser->match(Lexer::Kind::DECL_MUT);
  else if (annotated) parser->report(parser->previous(), 2000200);

  // attempt getting the associated identifier to be used
  auto *name = m_label(parser);
  if (name == nullptr) return nullptr;

  // determine whether or not optional now
  auto optional = parser->match(Lexer::Kind::PUNC_QUERY);

  // and then check for any incoming hints to be parsed
  auto colon = parser->match(Lexer::Kind::PUNC_COLON);
  Syntax::Annotation *hint = colon ? m_annotation(parser) : nullptr;
  if (hint == nullptr && colon) return nullptr; // ensure annotated

  // check for assignment of the variable now as necessary
  bool assign = !annotated && parser->match(Lexer::Kind::ASOP_DEF);
  auto *initializer = assign ? m_expression(parser) : nullptr;
  if (initializer == nullptr && assign) return nullptr;

  // ensure all our segments of the variable are valid now
  auto *variable = parser->allocate<Syntax::Variable>(snapshot.enclose(name), name->lexeme(), hint, initializer);

  // update the modifiers as necessary now
  if (optional) variable->modifiers().set(Variable::Flag::OPTIONAL);
  if (!immutable) variable->modifiers().set(Variable::Flag::MUTABLE);
  if (disposable) variable->modifiers().set(Variable::Flag::DISPOSABLE);

  // and return the resulting variable declaration
  return variable;
}

SABRE_MM_PARSE_DECL(Variable, parser) { return m_variable(parser, false); }
