/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Import, node, compiler, destination) {
  // ensure we trace the import execution
  $_UNUSED $_AUTO = compiler->trace(node);

  // get some of the baseline details
  auto exported = node->exported();
  auto patterns = node->patterns();
  auto *captures = compiler->captures();
  auto *registers = compiler->registers();

  // handle empty patterns a little differently than the rest
  if (patterns.empty()) {
    compiler->import(Register::Accumulator, node->path());
    if (!exported) return; // importing so no need to barrel exports
    return compiler->emit<Glyph::MODULE_BARREL>(Register::Accumulator);
  }

  // prepare the wildcard details to be used
  auto wildcard = Bytecode::Declaration({}, false);

  // if we have a wildcard import, then ensure we store properly
  if (node->wildcard()) wildcard = compiler->declare(patterns.at(0));

  // prepare the sink for the import to go to
  if (wildcard.second) destination = Register::Accumulator;
  else if (node->wildcard()) destination = wildcard.first;
  else if (destination.nowhere()) destination = Register::Accumulator;

  // attempt handling the incoming import now
  compiler->import(destination, node->path());

  // if exported then we need to re-export at our desired location
  if (exported) {
    auto name = compiler->string(patterns.at(0)->name()); // prepare
    return compiler->emit<Glyph::MODULE_EXPORT>(destination, name);
  }

  // if leaked, then we need to store properly
  if (wildcard.second) compiler->emit<Glyph::STORE_CONTEXT>(wildcard.first, destination);

  // stop if there are no patterns or just a wildcard handler
  if (patterns.empty() || node->wildcard()) return;

  // declare all the patterns to be available
  for (const auto &pattern : patterns) {
    $_UNUSED $_AUTO = compiler->trace(pattern);
    auto [vreg, leaked] = compiler->declare(pattern);

    // ensure we get some suitably details to be used
    auto index = compiler->symbol(pattern->name());
    auto treg = leaked ? registers->temporary() : vreg;

    // we attempt loading the field from the module and store it
    compiler->emit<Glyph::LOAD_FIELD>(treg, destination, index);
    compiler->store(pattern->name(), captures->extent(pattern), treg);
  }
}
