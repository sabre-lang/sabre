/// Sabre Includes
#include "sabre/variable/visitor.hpp"

//  PRIVATE METHODS  //

void Sabre::Variable::Analyzer::m_close() { m_close(m_outer); }
void Sabre::Variable::Analyzer::m_close(Depth depth) {
  auto deferred = std::move(m_deferred);
  for (auto &deferrer : deferred) deferrer(this);
  m_scopes.pop_back(), m_outer = depth;
}

void Sabre::Variable::Analyzer::m_reset(Syntax::Tree *node) { m_captures = node->captures(); }
void Sabre::Variable::Analyzer::m_visit(const Syntax::Node *node) {
  if ($_LIKELY(node != nullptr)) Visitor::visit(node, this);
}

Sabre::Variable::Analyzer::Lookup Sabre::Variable::Analyzer::m_lookup(const $::String::View &name) noexcept {
  // iterate through the scopes backwards with a depth index as well
  for (Depth ii = m_scopes.size() - 1; ii > -1; --ii) {
    const auto &scope = m_scopes.at(ii);
    auto iter = scope.find(name);
    if (iter == scope.cend()) continue;
    return {iter->second, ii};
  }

  // since the declaration found nothing, declare as a global
  return {0, -1};
}
