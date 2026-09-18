/// Relint Includes
#include "sabre/relint/_inline/rules.ipp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

//  MACROS  //

#define MM_RULE_NAME Sabre::Relint::Label::NO_UNSAFE_ASSIGNMENTS

//  PUBLIC METHODS  //

template <> void Sabre::Relint::Register<MM_RULE_NAME>::subscribe(Hooks *hooks, const Rule *rule) noexcept {
  hooks->overload<Syntax::Variable>([rule](const Syntax::Variable *variable, Context *context) {
    // prepare the mirrors and initializer
    auto *mirrors = context->mirrors();

    // get the incoming initializer to be used
    const auto *initializer = variable->initializer();

    // stop if there is no initializer
    if (initializer == nullptr) return;

    // prepare the incoming types to be used now
    auto expected = mirrors->resolve(variable)->type();
    auto incoming = mirrors->resolve(initializer)->type();

    // declare the error if possible to do so
    auto unsafe = !expected->is<Type::Any>() && incoming->is<Type::Any>();
    if (unsafe) context->report(initializer, rule, *expected); // show
  });
}

template <> Sabre::Relint::Rule Sabre::Relint::Register<MM_RULE_NAME>::define() noexcept {
  return {
      .recommended = true,
      .severity = Severity::WARN,
      .factory = subscribe,
      .documentation = {
          .message = "Unsafe assignment of an 'Any' value to variable of type '{0}'",
          .description = "Disallows assigning a value with type 'Any' to variables",
      },
  };
}
