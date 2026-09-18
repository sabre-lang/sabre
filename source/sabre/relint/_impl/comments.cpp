/// Sabre Includes
#include "sabre/relint/mirror.hpp"

//  PRIVATE METHODS  //

std::vector<$::String::View> Sabre::Relint::Comments::m_read(const Mirror *mirror) const {
  // stop if the baseline node is invalid
  if ($_UNLIKELY(mirror == nullptr)) return {};

  // get the baseline tree node to be used
  auto *tree = mirror->parent<Syntax::Tree>();
  if ($_UNLIKELY(tree == nullptr)) return {};

  // we should now have our necessary bounds
  auto &limits = tree->trivia()->bounds(), &bounds = mirror->bounds();

  // stop if there are any invalid conditions
  if ($_UNLIKELY(limits.m_left == nullptr)) return {};
  if ($_UNLIKELY(bounds.m_left == nullptr)) return {};

  // decrement from the left-most bounds whilst possible
  auto *front = bounds.m_left - 1, *outer = limits.m_left;

  // allow eating leading tokens depending on context
  if (!m_skip(front, outer)) return {};

  // bind our starting point for resolution now
  auto *start = front + 1;

  // iterate backwards whilst possible to do so
  do { --front; } while (m_validate(front, outer, false));

  // prepare a set of tokens that we can now collate
  std::span<const Lexer::Token> tokens = {front + 1, start};

  // and resolve these into their lexemes now
  auto predicate = [&](const Lexer::Token &token) { return m_trim(token); };
  return $::Ranges::To(tokens | std::views::transform(predicate));
}

$::String::View Sabre::Relint::Comments::m_trim(const Lexer::Token &token) const {
  // get the comment to be transformed
  auto comment = token.lexeme();

  // trim leading edge of our comment based on the incoming details
  if (comment.starts_with(m_prefix)) comment = comment.substr(m_prefix.size());

  // trim both ends of the comment before returning
  return $::Trim::both(comment);
}

bool Sabre::Relint::Comments::m_skip(const Lexer::Token *&token, const Lexer::Token *outer) const {
  // trim any leading trivia (eg: modifiers for declarations)
  while (token >= outer && token->flags().test(Lexer::Flag::MODIFIER)) --token;

  // trim any leading whitespace (eg: before the comment instance)
  while (m_validate(token, outer, true)) --token;

  // and force a final validation to occur
  return m_validate(token, outer, false);
}

bool Sabre::Relint::Comments::m_validate(const Lexer::Token *token, const Lexer::Token *outer, bool flag) const {
  return token >= outer && token->kind() == Lexer::Kind::MISC_CMT && !token->lexeme().starts_with(m_prefix) == flag;
}
