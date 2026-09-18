/// Syntax Includes
#include "sabre/syntax/_inline/expression.ipp"

//  PRIVATE METHODS  //

SABRE_MM_PARSE_INFIX(Execute, parser, callee, assignable) {
  // ensure we have a leading colon found
  m_assert(parser->advance(), Lexer::Kind::PUNC_POLICY);

  // prepare the snapshot instance now
  auto snapshot = parser->snapshot().offset(callee);

  // attempt parsing the incoming policy to be used now
  auto *token = m_label(parser);
  if (token == nullptr) return nullptr;

  // prepare a baseline policy value now
  auto policy = Function::Policy::ASYNC;

// validate the incoming policy name
#define X(P, N, ...)                                         \
  case XH::FNV::U32(N): policy = Function::Policy::P; break;
  switch (XH::FNV::U32(token->lexeme())) {
    SABRE_XX_FUNCTION_POLICIES(X) // parse here now
  default: parser->report(token, 2000103); break;
  }
#undef X

  // attempt constructing the call instance now as necessary
  auto result = Dispatch::m_infix<Syntax::Call>(parser, callee, assignable);

  // stop early if the result is a failure at all
  if (result == nullptr) return nullptr;

  // and construct the resulting call instance
  return parser->allocate<Syntax::Execute>(snapshot.enclose(token), result->as<Syntax::Call>(), policy);
}
