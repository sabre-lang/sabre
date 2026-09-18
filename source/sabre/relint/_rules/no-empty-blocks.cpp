/// Relint Includes
#include "sabre/relint/_inline/rules.ipp"

/// Syntax Includes
#include "sabre/syntax/statement/block.hpp"

//  MACROS  //

#define MM_RULE_NAME Sabre::Relint::Label::NO_EMPTY_BLOCKS

//  PUBLIC METHODS  //

template <> void Sabre::Relint::Register<MM_RULE_NAME>::subscribe(Hooks *hooks, const Rule *rule) noexcept {
  hooks->overload<Syntax::Block>([rule](const Syntax::Block *block, Context *context) {
    if (block->empty()) context->report(block->trivia()->bounds().outer(), rule, "block");
  });
}

template <> Sabre::Relint::Rule Sabre::Relint::Register<MM_RULE_NAME>::define() noexcept {
  return {
      .recommended = true,
      .severity = Severity::WARN,
      .factory = subscribe,
      .documentation = {
          .message = "Empty {0} statement",
          .description = "Disallow empty block statements",
      },
  };
}
