/// Syntax Includes
#include "sabre/syntax/_inline/expression.ipp"
#include "sabre/syntax/_inline/generator.ipp"

//  PRIVATE METHODS  //

Sabre::Syntax::Expression *Sabre::Parser::Dispatch::m_expression(Stream *parser, Precedence rank) {
  static constexpr auto s_bail = [](Stream *parser, auto... args) { return parser->report(args...); };
  return Syntax::Generator::expression<Syntax::Expression, Stream, Dispatch>(parser, rank, s_bail);
}
