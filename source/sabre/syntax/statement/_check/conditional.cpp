/// Sabre Includes
#include "sabre/variable/visitor.hpp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Conditional, node, analyzer) {
  analyzer->visit(node->condition());
  analyzer->visit(node->consequence());
  analyzer->visit(node->alternative());
}

SABRE_MM_CHECK_NODE(Conditional, node, analyzer) {
  // run the baseline deduction on the node firstly
  auto deduction = analyzer->check(node->condition());
  if (deduction.type->is<Type::Poison>()) return deduction;

  // get the truthiness to be used now
  auto truthiness = deduction.type->truthiness();

  // prepare a branch factory to be constructor
  auto branch = [&](const Syntax::Node *statement, bool primary) -> Type::Branch {
    // stop if the statement is not available for checking
    if (statement == nullptr) return [](Type::Analyzer *_) { return _->passable().flow; };
    else if (truthiness.indeterminate()) return [statement](auto *_) { return _->check(statement).flow; };
    else if (truthiness.roughly(primary)) return [statement](auto *_) { return _->check(statement).flow; };

    analyzer->redundant(statement); // declare as redundant now and show condition as necessary
    analyzer->report(node->condition()->trivia()->bounds(), 3000600, !primary ? "truth" : "false");

    auto depth = analyzer->depth(); // get depth before forcing a check
    return [=](auto *_) { return _->check(statement), _->unreachable(depth).flow; };
  };

  // prepare the branch handlers to be used now within the analyzer branch
  auto left = branch(node->consequence(), true), right = branch(node->alternative(), false);
  return analyzer->branch(std::move(deduction), std::move(left), std::move(right));
}
