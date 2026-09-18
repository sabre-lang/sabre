/// Syntax Includes
#include "sabre/syntax/_inline/annotation.ipp"

//  PRIVATE METHODS  //

Sabre::Syntax::Parameters *Sabre::Parser::Dispatch::m_parameters(Stream *parser, bool annotated) {
  // if there are no parameters then return them as currently empty
  auto empty = !parser->match(Lexer::Kind::PUNC_LPAREN) || parser->match(Lexer::Kind::PUNC_RPAREN);
  if (empty) return parser->allocate<Syntax::Parameters>(); // can safely remove our parameters now

  bool packed = false; // prepare initial details
  auto parameters = Syntax::Parameters::List();

  // attempt eating all the available parameters
  do {
    // stop early for trailing commas
    if (parser->check(Lexer::Kind::PUNC_RPAREN)) break;

    auto spread = parser->match(Lexer::Kind::PUNC_SPREAD);
    auto *parameter = m_variable(parser, annotated);

    // check the parameter succeeded before continuing
    if (parameter == nullptr) return nullptr;

    // handle all the parameter conditions to be used now
    if (packed) parser->report(parameter, spread ? 2000301 : 2000302);
    else if (spread) packed = true; // cache the incoming packing details

    // cannot have both spread/optional parametrs
    if (spread && parameter->optional()) {
      parser->report(parameter, 2000304); // declare an error
      parameter->modifiers().flip(Variable::Flag::OPTIONAL);
    }

    // emplace the parameter instance now
    parameters.emplace_back(parameter);

    // ensure that we are checking non-annotated code first
    if (annotated) continue;

    // if the initializer is given, then we ignore anything now
    if (parameter->initializer() == nullptr) continue;

    // ensure that we cannot have nodes after the spread now
    if (spread) parser->report(parameter, 2000303);
    else if (parameter->optional()) parser->report(parameter, 2000305);
  } while (parser->match(Lexer::Kind::PUNC_COMMA));

  // ensure there is a final parenthesis value as well
  if (!parser->expect(Lexer::Kind::PUNC_RPAREN)) return nullptr;

  // we can construct our parameters based on the packed details
  if (!packed) return parser->allocate<Syntax::Parameters>(parameters);
  else return parser->allocate<Syntax::Parameters::Spread>(parameters);
}

SABRE_MM_PARSE_HINT(Parameters, parser) { return m_parameters(parser, true); }
SABRE_MM_PARSE_DECL(Parameters, parser) { return m_parameters(parser, false); }
