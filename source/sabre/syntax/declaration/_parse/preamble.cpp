/// Syntax Includes
#include "sabre/syntax/_inline/declaration.ipp"

//  PRIVATE METHODS  //

Sabre::Syntax::Declaration *Sabre::Parser::Dispatch::m_subject(Stream *parser, Extent extent) {
  // handle immediate declarations (always valid)
  if (parser->check(Lexer::Flag::DEFINES)) return m_declaration<Syntax::Variable>(parser);

  // prepare the current token being used
  auto *token = parser->current();

  auto module = extent == Extent::MODULE; // prepare the details
  auto detail = module ? "a top-level declaration" : "a class field";

  // attempt resolving a suitable incoming set of details
  auto *declaration = [&] -> Syntax::Declaration * {
    switch (token->kind()) {
    case Lexer::Kind::DECL_ENUM: return m_declaration<Syntax::Enum>(parser);
    case Lexer::Kind::DECL_TYPE: return m_declaration<Syntax::Alias>(parser);
    case Lexer::Kind::DECL_CLASS: return m_declaration<Syntax::Class>(parser);
    case Lexer::Kind::DECL_SPACE: return m_declaration<Syntax::Namespace>(parser);
    default: return parser->report(token, 2000100, detail); // failed to validate
    }
  }();

  // fail-fast if necessary to do so
  if (declaration == nullptr) return nullptr;

  // check that we expected a class fields only now
  if (extent == Extent::CLASS) return parser->report(token, 2000100, "a class field");

  // validate namespaces are in the correct extent as well
  return module || !declaration->is<Syntax::Namespace>() ? declaration : parser->report(token, 2000100, detail);
}

Sabre::Syntax::Declaration *Sabre::Parser::Dispatch::m_preamble(Stream *parser, Extent extent) {
  // prepare the output decorator and attribute containers
  auto attributes = std::vector<Syntax::Attribute *>();
  auto decorators = std::vector<Syntax::Decorator *>();

  // attempt parsing attributes and decorators whilst possible to do so
  while (parser->check(Lexer::Flag::DECORATES)) {
    if (parser->check(Lexer::Kind::PUNC_ATTR)) attributes.emplace_back(m_attribute(parser));
    else if (parser->check(Lexer::Kind::PUNC_DECOR)) decorators.emplace_back(m_decorator(parser));
  }

  // check if either sets where incorrect at all
  auto invalid = std::ranges::contains(decorators, nullptr) || std::ranges::contains(attributes, nullptr);

  // attempt parsing all the available modifiers now
  auto *declaration = m_modifiers(parser, extent);
  if (invalid || declaration == nullptr) return nullptr;

  // check for valid preamble items now
  auto valid = declaration->is<Syntax::Variable, Syntax::Class>();
  auto *preamble = valid ? static_cast<Syntax::Preamble *>(declaration) : nullptr;

  // should be able to update the declaration now (if a preamble)
  if (preamble) preamble->decorators() = decorators, preamble->attributes() = attributes;
  else if (decorators.size() || attributes.size()) parser->report(declaration, 2000901);

  // and return the result
  return declaration;
}

Sabre::Syntax::Declaration *Sabre::Parser::Dispatch::m_modifiers(Stream *parser, Extent extent) {
  // prepare the modifiers output to be used
  auto accessibility = false;
  auto modifiers = Variable::Modifiers();

  // prepare a handler for setting modifiers
  auto emplace = [&](Variable::Flag flag, const Lexer::Token *token) {
    if ($_LIKELY(!modifiers.test(flag))) modifiers.set(flag);
    else parser->report(token, 2000900, token->lexeme());
  };

  // prepare a handler for class properties
  auto property = [&](const Lexer::Token *token) {
    // check if the token sets an accessibility flag at all
    if (token->flags().test(Lexer::Flag::ACCESSOR)) {
      if (accessibility) parser->report(token, 2000905);
      accessibility = true; // cache the base accessor
    }

    // otherwise set the basic details now
    if (extent == Extent::MODULE) parser->report(token, 2000903, token->lexeme());
    else if (extent == Extent::SCOPING) parser->report(token, 2000904, token->lexeme());
  };

  // prepare a handler for export modifiers
  auto exports = [&](const Lexer::Token *token) {
    if (extent == Extent::CLASS) parser->report(token, 2000902, token->lexeme());
    else if (extent != Extent::MODULE) parser->report(token, 2000904, token->lexeme());
  };

  // attempt matching as many modifiers as possible now
  while (parser->match(Lexer::Flag::MODIFIER)) {
    switch (auto *token = parser->previous(); token->kind()) {
#define SABRE_XX_TOKEN_MODIFIER(K, N, ...)                                              \
  case Lexer::Kind::MOD_##K: emplace(Variable::Flag::K, token), property(token); break;
#include "sabre/lexer/_defines/tokens.def"

    // "export" modifiers need to be handled different
    case Lexer::Kind::MOD_EXPORT: emplace(Variable::Flag::EXPORT, token), exports(token); break;

    // all other tokens should be unreachable
    default: $_ABORT("Unknown modifier token '{0}'", token->lexeme()); break;
    }
  }

  // attempt parsing a suitable subject now
  auto *declaration = m_subject(parser, extent);
  if (declaration == nullptr) return nullptr;

  // update the declarations modifiers
  declaration->modifiers() |= modifiers;

  // and return the resulting declaration
  return declaration;
}
