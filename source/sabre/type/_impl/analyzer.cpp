/// Sabre Includes
#include "sabre/relint/analyzer.hpp"
#include "sabre/flow/unreachable.hpp"
#include "sabre/globals/service.hpp"
#include "sabre/import/service.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/syntax/tree.hpp"

/// Syntax Includes
#include "sabre/syntax/annotation/generics.hpp"
#include "sabre/syntax/declaration/variable.hpp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

//  PROPERTIES  //

/// @brief Failure context to be used.
static auto g_failure = Sabre::Type::Exports(Sabre::Type::New::fail());

//  CONSTRUCTORS  //

Sabre::Type::Analyzer::Analyzer() : Analyzer($::Global::get<Runtime::Container>()) {}
Sabre::Type::Analyzer::Analyzer(XI::Container *services) : m_services(services) {
  m_world = m_services->get<Globals::Service>()->types();
}

//  PUBLIC METHODS  //

$::Unique::Pointer<Sabre::Type::World> Sabre::Type::Analyzer::scope(
    const Syntax::Constructor *constructor,
    const $::Shared::Pointer<Callable> &callable,
    const $::Shared::Pointer<Generic> &generic
) {
  // scope a new world to be used now
  auto world = scope();

  auto parameters = callable->parameters(); // get the base details now
  auto spread = callable->packed() ? New::list(parameters.back().value()) : nullptr;

  // pre-define any generic type parameters now
  for (const auto &constraint : generic ? generic->parameters() : Template()) {
    auto entity = world->types().declare(constraint->name(), constraint);
    if (entity == nullptr) report(4000400, constraint->name()); // dupe
  }

  // resolve all the necessary arguments now
  for (const auto &[ii, parameter] : $::Ranges::Each(constructor->parameters()->list())) {
    // attempt getting the current details to be used
    auto optional = parameter->optional();
    auto packed = parameter == constructor->spread();

    // prepare the incoming type to be used now
    auto type = packed ? spread : parameters.at(ii).value();

    // stop on invalid parameter types
    if (type == nullptr) break;

    // if we are in spread-mode, then change to an array
    if (optional) type = New::maybe(type);

    // and attempt declaring the incoming entity now
    auto *entity = world->values().declare(sanity(parameter), type);
    if (entity == nullptr) report(parameter, 4000401, parameter->name());
  }

  // return the current scoping
  return world;
}

std::pair<$::String::View, Sabre::Type::Erased> Sabre::Type::Analyzer::vardef(const Syntax::Variable *variable) {
  // get the internal name of the variable
  auto name = variable->name();

  // prepare the expected type based on the given annotation
  auto expected = check(variable->hint()).type;

  // prepare the initializer type based on what we have currently
  auto initializer = check(variable->initializer()).type;
  if (expected->is<Unset>()) expected = initializer;
  if (expected->is<Unset>()) expected = New::any();

  // attempt unifying the value now if the expected value is not "any"
  while (!expected->is<Any>() && variable->initializer()) {
    if (variable->optional() && initializer->is<None>()) break;
    else if (expected->unify(initializer)) break; // valid typing
    return {name, report(variable, 3000300, *initializer, *expected).type};
  }

  // modify the expecting typing based on the optionality
  if (variable->optional()) expected = New::maybe(expected);

  // return the resulting type reference now
  return {name, expected};
}

const Sabre::Syntax::Declaration *Sabre::Type::Analyzer::sanity(const Syntax::Declaration *node) {
  // get the incoming node name
  auto name = node->name();

  // get the entity instance to be checked against
  auto *entity = m_world->lookup(name);
  if (entity == nullptr) return node;

  // ignore if our current export flags do not have a mismatch.
  if (entity->exported() == node->modifiers().test(Variable::Flag::EXPORT)) return node;

  // since there is a mismatch we can report this merge error now
  report(8000203, name);

  // also resolve the current entity details
  auto iter = m_world->m_ranges.find(name);
  if (iter == m_world->m_ranges.end()) return node;

  // can safely report the mismatch at the origin as well
  return report(iter->second, 8000203, name), node;
}

Sabre::Type::Deduction Sabre::Type::Analyzer::redundant(const Syntax::Node *node) {
  return report(node->trivia()->range(), 4000900);
}

void Sabre::Type::Analyzer::deprecated(Entity *entity, const Syntax::Node *node) {
  entity->unused(false); // always mark the entity as unused too
  if (entity->deprecated()) report(node, 9000100, entity->notice());
}

Sabre::Type::Entity *Sabre::Type::Analyzer::preamble() const noexcept {
  auto *entity = m_world->preamble();
  return entity->unused(false), entity;
}

Sabre::Type::Deduction Sabre::Type::Analyzer::preamble(const Syntax::Preamble *preamble, Entity *entity) {
  // set the entity as the preamble target
  m_world->m_preamble = entity;

  // iterate over the available attributes and decorators
  for (const auto *attribute : preamble->attributes()) check(attribute);
  for (const auto *decorator : preamble->decorators()) check(decorator);

  // unset the current preamble target now
  m_world->m_preamble = nullptr;

  // and return the resulting type deduction
  return passable(entity->value());
}

Sabre::Type::Deduction Sabre::Type::Analyzer::passable(const Erased &type) const { return type; }

Sabre::Type::Deduction Sabre::Type::Analyzer::unreachable(Flow::Effect effect) const {
  return unreachable(depth(), effect);
}

Sabre::Type::Deduction Sabre::Type::Analyzer::unreachable(Flow::Degree degree, Flow::Effect effect) const {
  return Deduction(New::never(), $::Unique::New<Flow::Unreachable>(degree, effect));
}

Sabre::Type::Deduction Sabre::Type::Analyzer::check(const Syntax::Node *node, const Erased &fallback) {
  // stop early if the underlying node is invalid
  if ($_UNLIKELY(node == nullptr)) return fallback;

  // and resolve the final deduction to be used
  auto deduction = Visitor::visit(node, this);

  // update the current typing now
  node->trivia()->type() = deduction.type;

  // and return the resulting deduction
  return deduction;
}

Sabre::Type::Deduction Sabre::Type::Analyzer::check(const std::vector<Syntax::Node *> &nodes) {
  Flow::Degree degree = -1, minimum = depth(); // get the current degree details
  auto propagate = [minimum](Flow::Degree _) { return _ > -1 && _ <= minimum; };

  // attempt checking all the incoming statements now
  for (const auto &node : nodes) {
    if (propagate(degree)) redundant(node), check(node);
    else degree = check(node).flow->degree(); // update
  }

  // ensure that the instance is passable or not
  return propagate(degree) ? unreachable(degree) : passable();
}

void Sabre::Type::Analyzer::check(const std::vector<Syntax::Expression *> &nodes) {
  for (const auto *node : nodes) check(node);
}

Sabre::Type::Template Sabre::Type::Analyzer::check(const Syntax::Template &parameters) {
  static constexpr auto cast = [](Erased &&node) { return std::static_pointer_cast<Parameter>(node); };
  auto predicate = [&](const Syntax::Placeholder *parameter) { return cast(check(parameter).type); };
  return $::Ranges::To(parameters | std::views::transform(predicate)); // can safely transform here
}

$::Unique::Pointer<Sabre::Type::Exports>
Sabre::Type::Analyzer::audit(const Syntax::Tree *tree, Diagnostic::Reporter *reporter) {
  // check if the initial syntax failed to properly be typed
  if (reporter->failed()) return $::Unique::New<Exports>(New::fail());

  // attempt running our auditing now as needed
  m_world->m_deferred.clear(), m_reporter = reporter;

  // prepare a formatted label to be used
  auto label = fmt::format("import \"{0}\"", tree->resource().relative());

  // bind the baseline exports to be used
  m_exports = $::Unique::New<Exports>(label);

  // and finally return the resulting now
  return check(tree), m_reporter = nullptr, std::move(m_exports);
}

Sabre::Type::Erased Sabre::Type::Analyzer::instantiate(const Erased &type, const Syntax::Specialization &arguments) {
  // we need to resolve a suitable generic target
  auto target = type->is<Transform>() ? type->as<Transform>()->reduce() : type;

  // ensure the type is actually generic before continuing with instantiating
  if (!target->is<Generic>()) return arguments.empty() ? target : report(3000350, *target).type;

  // cast to the generic we want to instantiate
  auto generic = target->as<Generic>();

  // pre-check the arity and adicity of the generic
  auto total = arguments.size();
  auto arity = generic->arity();
  auto adicity = generic->adicity();

  // ensure the totals are suitably valid before continuing
  if (total < arity) return report(3000401, arity).type;
  else if (total > adicity) return report(3000402, adicity).type;

  // and attempt mapping the types into the desired format
  auto predicate = [&](const Syntax::Annotation *argument) { return check(argument).type; };
  return generic->instantiate($::Ranges::To(arguments | std::views::transform(predicate)));
}

Sabre::Type::Control Sabre::Type::Analyzer::merge(Control &&left, Control &&right) const noexcept {
  // get the current degree values to be used
  auto ld = left->degree(), rd = right->degree();
  auto lp = left->passable(), rp = right->passable();

  // if either are passable, then we succeed our merge
  if (lp || rp) return $::Unique::New<Flow::Passable>();

  // otherwise both are unreachable, so we push the most suitable
  return std::max(ld, rd) == rd ? std::move(left) : std::move(right);
}

Sabre::Type::Deduction Sabre::Type::Analyzer::branch(Deduction &&deduction, Branch &&left, Branch &&right) {
  /// TODO: prepare the necessary narrowings to be used
  //   auto narrowings = deduction.narrowings;
  //   auto inverted = m_invert(narrowings);

  // execute our seperate branches now with their narrowings
  auto lf = m_using(std::move(left)), rf = m_using(std::move(right));

  // stop our deductions if the given one never succeeded
  if (deduction.flow->unreachable()) return std::move(deduction);

  // finally construct our resulting deduction to be used
  return Deduction(New::any(), merge(std::move(lf), std::move(rf)));
}

const Sabre::Type::Exports *Sabre::Type::Analyzer::import(const $::String::View &path, const XLSP::Range &range) {
  Import::Service *modules = *m_services; // resolve now
  auto result = modules->resolve(path, resource().body());
  if (result.has_value()) return import(*result, range);
  return report(range, 8000000, result.error()), &g_failure;
}

const Sabre::Type::Exports *Sabre::Type::Analyzer::import(const $::URI::Buffer &resource, const XLSP::Range &) {
  // prepare the importer service to be used
  Import::Service *modules = *m_services;

  // ignore if we could not validly fetch our module
  auto *found = modules->fetch(resource);
  if (found == nullptr) return &g_failure;

  // should be able to analyze the module now
  found->analyze(m_services);

  // and return the underlying type-context
  return found->metadata<Module::Phase::TYPED>()->exports().get();
}

//  PRIVATE METHODS  //

Sabre::Type::Control Sabre::Type::Analyzer::m_using(Branch &&branch) {
  /// TODO: we bind deductions to the current world

  // attempt running our branch now
  auto flow = branch(this);

  /// TODO: post-update after using required deductions

  // and return the final flow
  return flow;
}
