/// Sabre Includes
#include "sabre/type/world.hpp"
#include "sabre/type/analyzer.hpp"

//  CONSTRUCTORS  //

Sabre::Type::World::World(Analyzer *analyzer) : World(analyzer->m_world, analyzer) {}
Sabre::Type::World::World(World *parent, Analyzer *analyzer) :
    m_depth(($_ASSERT(parent), parent->m_depth + 1)), m_parent(parent), m_analyzer(analyzer) {
  if (m_analyzer) m_analyzer->m_world = this;
}

Sabre::Type::World::~World() {
  // ignore if there is no suitable analyzer instance held
  if (m_analyzer == nullptr) return;

  // run all the deferred callback handlers
  for (auto &callback : m_deferred) callback(m_analyzer);

  // ensure we report any errors relating to unused variables
  for (const auto &[name, entity] : m_entities) {
    // resolve the underlying location of the entity
    auto range = m_ranges.at(name);

    // check whether we should actually report the instance
    if (!entity.unused() || name.starts_with('_') || range == XLSP::Range()) continue;

    // if unused, then we declare as such to the reporter
    m_analyzer->report(range, entity.transient() ? 4000901 : 4000902, name);
  }

  // ensure we resolve the required analyzer parent
  m_analyzer->m_world = m_parent;
}
