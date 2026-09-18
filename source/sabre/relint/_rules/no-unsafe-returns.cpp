/// Relint Includes
#include "sabre/relint/_inline/rules.ipp"

/// Syntax Includes
#include "sabre/syntax/declaration/variable.hpp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Declarations
$_FWD(Sabre::Relint::Dispatch, bool unsafe_returns(const Mirror *, const Mirror *))

//  MACROS  //

#define MM_RULE_NAME Sabre::Relint::Label::NO_UNSAFE_RETURNS

//  PUBLIC METHODS  //

bool Sabre::Relint::Dispatch::unsafe_returns(const Mirror *value, const Mirror *parent) {
  // ensure both the value and parent are valid
  if (value == nullptr || parent == nullptr) return false;

  // prepare the incoming return typing now
  auto type = value->type();

  // pull out the signature and callable typing here
  auto callable = Type::New::cast<Type::Callable>(parent->type());
  auto returns = callable ? callable->returns() : Type::New::unset();

  // check if the incoming value is potentially unsafe
  return type->is<Type::Any>() && !returns->is<Type::Any>();

  //   // declare the error if possible to do so
  //   if (unsafe) context->report(value->origin(), rule);
}

template <> void Sabre::Relint::Register<MM_RULE_NAME>::subscribe(Hooks *hooks, const Rule *rule) noexcept {
  // check all base return values here
  hooks->overload<Syntax::Return>([rule](const Syntax::Return *node, Context *context) {
    // prepare the mirrors to be used here
    auto *mirrors = context->mirrors();
    auto *value = mirrors->resolve(node->value());

    // attempt scanning until the parent callable
    auto parent = mirrors->resolve(value->parent<Syntax::Lambda>());

    // attempt dispatching the unsafe handler
    if (Dispatch::unsafe_returns(value, parent)) context->report(value->origin(), rule);
  });

  // check all incoming lambdas (for non-block returns)
  hooks->overload<Syntax::Lambda>([rule](const Syntax::Lambda *lambda, Context *context) {
    // ignore if we have a block given
    if (lambda->body()->is<Syntax::Block>()) return;

    // prepare the mirrors to be used
    auto *mirrors = context->mirrors();

    // prepare the base mirrors now
    auto parent = mirrors->resolve(lambda);
    auto value = mirrors->resolve(lambda->body());

    // attempt dispatching the unsafe handler
    if (Dispatch::unsafe_returns(value, parent)) context->report(value->origin(), rule);
  });
}

template <> Sabre::Relint::Rule Sabre::Relint::Register<MM_RULE_NAME>::define() noexcept {
  return {
      .recommended = true,
      .severity = Severity::WARN,
      .factory = subscribe,
      .documentation = {
          .message = "Unsafe return of 'Any' value type",
          .description = "Disallows returning a value of type 'Any'",
      },
  };
}
