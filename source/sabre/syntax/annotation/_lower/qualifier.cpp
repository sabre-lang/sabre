/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Qualifier, node, compiler, destination) {
  // get the incoming segments to be handled
  const auto &segments = node->segments();

  // if the destination is nowhere, then we ignore
  if (segments.empty() || destination.nowhere()) return;

  // attempt loading in the qualifier segment by segment
  auto *identifier = segments.front();
  auto treg = compiler->registers()->temporary();

  // load the initial identifier as a normal value
  compiler->lower(identifier, destination);

  // iterate through the available fields now
  for (size_t ii = 1; ii < segments.size(); ++ii) {
    auto index = compiler->symbol(segments.at(ii)->name()); // load
    compiler->emit<Glyph::LOAD_FIELD>(destination, destination, index);
  }
}
