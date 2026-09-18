/// Syntax Includes
#include "sabre/syntax/_inline/annotation.ipp"
#include "sabre/syntax/_inline/declaration.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_DECL(Override, parser) {
  // prepare the snapshot to be used
  auto snapshot = parser->snapshot();

  // get the original token location to be used
  m_assert(parser->advance(), "Operator");

  // ensure there is now a period available
  if (!parser->expect(Lexer::Kind::PUNC_PERIOD)) return nullptr;

  // attempt parsing an incoming kind of operator
  auto kind = Operator::Kind::UNK;
  auto method = parser->current()->lexeme();

#define X(K, N, ...)                                                            \
  case XH::FNV::U32(N): kind = Operator::Kind::K; break; // check for operators
  switch (XH::FNV::U32(method)) { SABRE_XX_OPERATORS_CUSTOM(X) default : break; }
#undef X

  auto invalid = kind == Operator::Kind::UNK; // check validity
  invalid ? parser->report(2000801, method) : parser->advance();

  auto self = parser->check(Lexer::Kind::PUNC_RBRACK); // ignore value if none given
  auto *target = self ? parser->allocate<Syntax::Self>(snapshot.bounds()) : m_expression(parser);
  if (target == nullptr || invalid) return nullptr; // failed to parse a suitable target

  // fail if the kind is currently invalid
  return parser->allocate<Syntax::Override>(snapshot.bounds(), kind, target);
}
