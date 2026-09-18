/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Ternary, node, analyzer) {
  analyzer->visit(node->condition());
  analyzer->visit(node->consequence());
  analyzer->visit(node->alternative());
}

SABRE_MM_CHECK_NODE(Ternary, node, analyzer) {
  // trace from the ternary node
  $_UNUSED $_AUTO = analyzer->trace(node);

  auto deduction = analyzer->check(node->condition());
  auto truthiness = deduction.type->truthiness();

  // validate the consequence and alternative values now
  auto consequence = analyzer->check(node->consequence());
  auto alternative = analyzer->check(node->alternative());

  // stop early if the condition was not passable
  if (deduction.flow->unreachable()) return deduction;

  // handle branching our details based on the truthiness
  if (truthiness.roughly(true)) return consequence;
  if (truthiness.roughly(false)) return alternative;

  // otherwise we want to merge the results somehow
  return Type::New::merge(consequence.type, alternative.type);
}
