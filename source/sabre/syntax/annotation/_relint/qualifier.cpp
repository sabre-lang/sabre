/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Qualifier, node, analyzer) {
  // prepare the metadata and references
  auto *mirrors = analyzer->mirrors();
  auto *references = analyzer->references();

  // prepare the segments to be handled
  const auto &segments = node->segments();

  // start by getting the front-most segment
  auto *identifier = segments.front();

  // get the initial definition to be used
  auto definition = references->resolve(identifier->name());

  // we should always bind both definitions to the base items
  references->relate(mirrors->resolve(node), definition);
  references->relate(mirrors->resolve(identifier, node), definition);

  // we want to iterate through the segments now to resolve chained items
  for (size_t ii = 1; ii < segments.size(); ++ii) {
    // update our current details to be used
    identifier = segments.at(ii);

    // get the current parent definition
    auto *parent = definition->variable;

    // stop handling if there is no suitable variable
    if (parent == nullptr) return;

    // attempt finding a suitable definition
    definition = parent->lookup(identifier->name());

    // post-resolve the mirror for the identifier
    auto *mirror = mirrors->resolve(identifier, node);
    references->relate(mirror, definition);
  }
}
