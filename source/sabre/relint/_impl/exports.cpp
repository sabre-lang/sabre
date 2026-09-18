/// Sabre Includes
#include "sabre/relint/exports.hpp"

//  PUBLIC METHODS  //

bool Sabre::Relint::Exports::bound(const Syntax::Node *origin) const { return m_storage.contains(origin); }

const Sabre::Relint::Mirror *Sabre::Relint::Exports::resolve(const Syntax::Node *origin) const {
  auto mirror = m_storage.find(origin); // safe resolution
  return mirror == m_storage.end() ? nullptr : mirror->second.get();
}

Sabre::Relint::Mirror *Sabre::Relint::Exports::resolve(const Syntax::Node *origin, const Syntax::Node *parent) {
  auto iter = m_storage.find(origin); // pre-scan here
  if (iter != m_storage.end()) return iter->second.get();

  // check that our origin can be suitably sorted at all
  auto comparator = Syntax::Comparator();
  auto sortable = comparator.validate(origin);

  // emplace the origin onto the sorted values
  if (sortable) m_sorted.emplace_back(origin), std::ranges::sort(m_sorted, comparator);

  // then emplace the origin and mirror onto those available
  return m_storage.emplace(origin, m_create(origin, parent)).first->second.get();
}

const Sabre::Relint::Mirror *Sabre::Relint::Exports::search(const XLSP::Position &position) const noexcept {
  return search(position, [](const Syntax::Node *node) -> bool {
    // ensure we ignore some nodes (since we want internal values)
    switch (node->trivia()->hash()) {
    case $::RTTI::Hash<Syntax::Call>(): return false;
    case $::RTTI::Hash<Syntax::Tree>(): return false;
    case $::RTTI::Hash<Syntax::Block>(): return false;
    case $::RTTI::Hash<Syntax::Typed>(): return false;
    case $::RTTI::Hash<Syntax::Ternary>(): return false;
    default: return true; // valid target for searching
    }
  });
}

const Sabre::Relint::Mirror *
Sabre::Relint::Exports::search(const XLSP::Position &position, Filter &&filter) const noexcept {
  // if the filter is invalid, then request with the baseline
  if (filter == nullptr) return search(position);

  // attempt finding a suitable candidate to be used
  for (const auto *node : m_sorted) {
    if (filter(node) && node->trivia()->range().contains(position)) return resolve(node);
  }

  // otherwise we failed to find a candidate
  return nullptr;
}

//  PRIVATE METHODS  //

$::Unique::Pointer<Sabre::Relint::Mirror>
Sabre::Relint::Exports::m_create(const Syntax::Node *origin, const Syntax::Node *parent) const noexcept {
  if (parent == nullptr) return $::Unique::New<Mirror>(origin);
  else return $::Unique::New<Mirror>(origin, resolve(parent));
}
