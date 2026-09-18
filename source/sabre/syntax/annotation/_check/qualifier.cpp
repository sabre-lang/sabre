/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Qualifier, node, analyzer) {

  /**
   * We only visit the initial qualifier segment as this
   * is all we need for typical capture resolution (since
   * fields do not require capturing).
   */

  const auto &segments = node->segments(); // resolve now
  if (segments.size()) analyzer->visit(segments.front());
}

SABRE_MM_CHECK_NODE(Qualifier, node, analyzer) {
  // get the underlying segments
  const auto &segments = node->segments();

  // ensure that we actually have some segments to begin with
  $_ASSERT(!segments.empty(), "Qualifier segments cannot be empty");

  // get the current identifier to be used
  auto *identifier = segments.front();
  auto name = identifier->name();
  auto chain = $::String::Buffer(name);

  auto *entity = analyzer->world()->lookup(name); // get the current entity
  if (entity == nullptr) return analyzer->report(identifier, 4000300, chain);

  // update the entity reference flag now and mark the first identifier
  analyzer->deprecated(entity, identifier);

  // construct the iterator element now
  auto segment = *entity;

  // iterate through the available fields now
  for (size_t ii = 1; ii < segments.size(); ++ii) {
    // ensure the current entity is opaque and can be used
    if (!segment.opaque()) return analyzer->report(identifier, 3000200, chain);

    // update the previous identifiers typing now
    identifier->trivia()->type() = segment.type();

    // jump to the next available identifier and chain segment
    identifier = segments.at(ii), segment = segment.value()->lookup(identifier->name());

    // fail immediately if there is no valid reference
    if (segment.unset()) return analyzer->report(identifier, 4000200, chain, name);

    // post-update the current chain to be used
    chain += "." + $::String::Buffer(identifier->name());
  }

  // ensure the final segment we have is transient before inferring the type
  if (!segment.transient()) return analyzer->report(identifier, 3000201, chain);

  // ensure we trace errors here as well
  $_UNUSED $_AUTO = analyzer->trace(node);

  // bind the last identifiers associated typing as well
  auto base = identifier->trivia()->type() = segment.type();
  auto type = analyzer->instantiate(base, node->types());
  return analyzer->passable(type); // return the final result
}
