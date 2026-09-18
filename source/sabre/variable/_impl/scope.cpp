/// Sabre Includes
#include "sabre/variable/scope.hpp"

//  PUBLIC METHODS  //

Sabre::Variable::Upvalue Sabre::Variable::Scope::m_upvalue(const $::String::View &name, size_t depth) const {
  // prepare the incoming context to be used
  auto *context = this;

  // attempt closing over contexts if possible
  while (context && context->m_leaked.empty()) context = context->m_ancestor.get();

  // if we have no more context values, then we stop handling
  if (context == nullptr) return {.depth = SIZE_MAX, .slot = {}};

  // if we have a match on this context, then stop with the current instance
  if (context->m_leaked.contains(name)) return {.depth = depth, .slot = context->m_bound.at(name)};

  // stop if there is no ancestor available now
  if (context->m_ancestor == nullptr) return {.depth = SIZE_MAX, .slot = {}};

  // otherwise we declare the next context as a potential candidate
  return context->m_ancestor->m_upvalue(name, depth + 1);
}
