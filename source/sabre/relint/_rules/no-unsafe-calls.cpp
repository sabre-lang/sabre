/// Relint Includes
#include "sabre/relint/_inline/rules.ipp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

//  MACROS  //

#define MM_RULE_NAME Sabre::Relint::Label::NO_UNSAFE_CALLS

//  PUBLIC METHODS  //

template <> void Sabre::Relint::Register<MM_RULE_NAME>::subscribe(Hooks *hooks, const Rule *rule) noexcept {
  hooks->overload<Syntax::Call>([rule](const Syntax::Call *node, Context *context) {
    // prepare the mirrors and callee
    auto *callee = node->callee();
    auto *mirrors = context->mirrors();

    // get the incoming typing to be checked
    auto type = mirrors->resolve(callee)->type();

    // declare the error if possible to do so
    if (type->is<Type::Any>()) context->report(callee, rule);
  });
}

template <> Sabre::Relint::Rule Sabre::Relint::Register<MM_RULE_NAME>::define() noexcept {
  return {
      .recommended = true,
      .severity = Severity::WARN,
      .factory = subscribe,
      .documentation = {
          .message = "Unsafe call of 'Any' value type",
          .description = "Disallows calling a value with type 'Any'",
      },
  };
}
