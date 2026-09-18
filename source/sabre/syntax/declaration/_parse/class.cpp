/// Sabre Includes
#include "sabre/parser/delimited.hpp"

/// Syntax Includes
#include "sabre/syntax/_inline/declaration.ipp"
#include "sabre/syntax/_inline/expression.ipp"

//  PRIVATE METHODS  //

Sabre::Syntax::Expression *Sabre::Parser::Dispatch::m_extends(Stream *parser) {
  // prepare the underlying delimited/enclosed handlers
  static constexpr auto s_opening = Lexer::Kind::PUNC_LBRACK;
  static constexpr auto s_closing = Lexer::Kind::PUNC_RBRACK;
  static constexpr auto s_separator = Lexer::Kind::PUNC_PERIOD;
  static constexpr auto s_annotation = [](auto *_) { return m_annotation(_); };
  static constexpr auto s_identifier = [](auto *_) { return m_identifier(_); };
  static auto s_delimited = Delimited<Syntax::Identifier, s_separator>(s_identifier);
  static auto s_enclosed = Enclosed<Syntax::Annotation, s_opening, s_closing>(s_annotation);

  // cache the current range value to be used
  auto snapshot = parser->snapshot();

  // attempt parsing all the available segments now
  auto segments = s_delimited.dispatch(parser);
  if (!segments.has_value()) return nullptr;

  // fail if there is no immediate value to be resolved
  if (segments->empty()) return parser->report(2000100, "a super-class");

  auto templated = parser->check(s_opening); // check if we have any templated types
  auto types = templated ? s_enclosed.dispatch(parser) : std::vector<Syntax::Annotation *>();
  if (!types.has_value()) return nullptr; // failed if the above dispatcher did

  // report a failure if the types are empty at all
  if (templated && types->empty()) parser->report(snapshot.bounds(), 2000301);

  // should safely be able to construct our callee
  auto *callee = std::ranges::fold_left(
      *segments, nullptr, [&](Syntax::Expression *callee, auto *identifier) -> Syntax::Expression * {
        if (callee == nullptr) return identifier; // resolve baseline
        return parser->allocate<Syntax::Accessor>(snapshot.bounds(), identifier, callee);
      }
  );

  // update the callee if there are any templated values
  if (types->size()) callee = parser->allocate<Syntax::Typed>(snapshot.bounds(), callee, *types);

  // check if there is an incoming opening call at all
  auto open = parser->check(Lexer::Kind::PUNC_LPAREN);
  if (open) return m_infix<Syntax::Call>(parser, callee, false);
  else return parser->allocate<Syntax::Call>(snapshot.bounds(), callee);
}

Sabre::Syntax::Specialization Sabre::Parser::Dispatch::m_implements(Stream *parser) {
  // prepare the output specialization implements
  auto implements = Syntax::Specialization();

  // if there is no implementation keyword, then ignore finding
  if (!parser->match(Lexer::Kind::DECL_IMPL)) return implements;

  // if we have an immediate opening brace, then declare a warning
  if (parser->check(Lexer::Kind::PUNC_LBRACE)) return parser->report(2000300), implements;

  // attempt matching annotations whilst possible to do so
  do { implements.emplace_back(m_annotation(parser)); } while (parser->match(Lexer::Kind::PUNC_COMMA));

  // filter and ensure we return the resulting implementation values
  return std::erase_if(implements, [](auto *_) { return _ == nullptr; }), implements;
}

SABRE_MM_PARSE_DECL(Header, parser) {
  // attempt reading a constructor value to be used
  auto *constructor = m_declaration<Syntax::Constructor>(parser);
  if (constructor == nullptr) return nullptr; // failed to read

  // attempt parsing the incoming class constructor now
  auto extends = parser->match(Lexer::Kind::ARROW_BOLD);
  auto *super = extends ? m_extends(parser) : nullptr;
  if (super == nullptr && extends) return nullptr;

  // parse the implementation interfaces
  auto implements = m_implements(parser);

  // and construct the resulting header
  return parser->allocate<Syntax::Header>(constructor, static_cast<Syntax::Call *>(super), implements);
}

SABRE_MM_PARSE_DECL(Class, parser) {
  // prepare the parser snapshot
  auto snapshot = parser->snapshot();

  // ensure we have a leading class token
  m_assert(parser->advance(), Lexer::Kind::DECL_CLASS);

  // build the necessary identifier to be used now
  auto *name = m_label(parser);
  if (name == nullptr) return nullptr;

  // attempt parsing the incoming header
  auto *header = m_declaration<Syntax::Header>(parser);
  if (header == nullptr) return nullptr; // fail here

  // prepare the details for reading fields
  static constexpr auto s_opening = Lexer::Kind::PUNC_LBRACE;
  static constexpr auto s_closing = Lexer::Kind::PUNC_RBRACE;
  static constexpr auto s_separator = Lexer::Kind::PUNC_TERM;
  static constexpr auto s_callback = [](auto *_) -> Syntax::Declaration * {
    // resolve the baseline field instance
    auto *field = m_preamble(_, Extent::CLASS);
    if (field == nullptr) return nullptr;

    // get the underlying modifiers to be updated
    auto &modifiers = field->modifiers();

    // ignore modifiers that are already defined
    if (modifiers.test(Variable::Flag::PUBLIC, Variable::Flag::PROTECTED)) return field;

    // ensure we default the "private" accessibility flag
    return modifiers.set(Variable::Flag::PRIVATE), field;
  };

  // construct the enclosed handler to bs used now
  static auto s_enclosed = Enclosed<Syntax::Declaration, s_opening, s_closing, s_separator>(s_callback);

  // attempt reading the available fields now
  auto fields = s_enclosed.dispatch(parser);
  if (!fields.has_value()) return nullptr;

  // build the resulting location
  const auto bounds = snapshot.enclose(name);

  // construct a block to emplace these fields into
  auto *block = parser->allocate<Syntax::Block>(*fields);

  // and finally construct the resulting class instance
  return parser->allocate<Syntax::Class>(bounds, name->lexeme(), header, *fields, block);
}
