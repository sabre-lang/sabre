#ifndef _SABRE_TYPE_ANALYZER_HPP
#define _SABRE_TYPE_ANALYZER_HPP

/// Sabre Includes
#include "sabre/diagnostic/scope.hpp"
#include "sabre/shape/service.hpp"
#include "sabre/type/deduction.hpp"
#include "sabre/type/exports.hpp"
#include "sabre/type/world.hpp"

namespace Sabre::Type {

/// @brief Type-Checking Service.
class Analyzer : public XI::Transient {
  //  TYPEDEFS  //

  /// @brief Allow type-worlds internal access.
  friend class World;

  //  PROPERTIES  //

  /// @brief Services container.
  XI::Container *m_services;

  /// @brief Current scoped world instance.
  World *m_world = nullptr;

  /// @brief Diagnostic reporter instance.
  Diagnostic::Reporter *m_reporter = nullptr;

  /// @brief The current exports context.
  $::Unique::Pointer<Exports> m_exports = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an analyzer instance.
   * @param services                  Services container.
   */
  explicit Analyzer();
  explicit Analyzer(XI::Container *services);

  //  PUBLIC METHODS  //

  /// @brief Gets the currently scoped type-world.
  inline constexpr World *world() noexcept { return m_world; }
  inline constexpr const World *world() const noexcept { return m_world; }

  /// @brief Gets the exports content.
  inline constexpr Exports *exports() const noexcept { return m_exports.get(); }

  /// @brief Gets the shapes service instance.
  inline constexpr Shape::Service *shapes() const noexcept { return *m_services; }

  /// @brief Gets the depth of the currently scoped world.
  inline constexpr Flow::Degree depth() const noexcept { return m_world->depth(); }

  /// @brief Gets the resource of this analyzer.
  inline constexpr $::URI::View resource() const noexcept { return m_reporter->resource(); }

  /**
   * @brief Handles tracing reporter resources.
   * @param node                      Node to trace.
   */
  inline constexpr Diagnostic::Scope trace(const Syntax::Node *node) {
    return node ? trace(node->trivia()->range()) : Diagnostic::Scope();
  }

  /**
   * @brief Handles tracing reporter resources.
   * @param range                     Range to trace.
   */
  inline constexpr Diagnostic::Scope trace(const XLSP::Range &range) { return Diagnostic::Scope(m_reporter, range); }

  /// @brief Scopes a simply world instance.
  inline constexpr $::Unique::Pointer<World> scope() noexcept { return $::Unique::New<World>(this); }

  /**
   * @brief Handles scoping a world instance.
   * @param constructor               Function ctor.
   * @param callee                    Callee typing.
   * @param generic                   Generic typing.
   */
  $::Unique::Pointer<World> scope(
      const Syntax::Constructor *constructor,
      const $::Shared::Pointer<Callable> &callable,
      const $::Shared::Pointer<Generic> &generic
  );

  /**
   * @brief Handles sanity checks on declarations.
   * @param node                          Declaration node.
   */
  const Syntax::Declaration *sanity(const Syntax::Declaration *node);

  /**
   * @brief Declares a variable.
   * @param variable                      Variable to declare.
   */
  std::pair<$::String::View, Erased> vardef(const Syntax::Variable *variable);

  /**
   * @brief Handles deferring type-checking.
   * @param deduction                 Passthrough result.
   * @param deferrer                  Deferred handler.
   */
  inline constexpr Deduction defer(Deferrer &&deferrer) { return defer(passable(), std::move(deferrer)); }
  inline constexpr Deduction defer(Deduction &&deduction, Deferrer &&deferrer) {
    return m_world->m_deferred.emplace_back(std::move(deferrer)), std::move(deduction);
  }

  /**
   * @brief Constructs a passable control-flow.
   * @param type                      Result type.
   */
  Deduction passable(const Erased &type = New::none()) const;

  /**
   * @brief Declares code as unreachable/redundant.
   * @param node                      Node to make redundant.
   */
  Deduction redundant(const Syntax::Node *node);

  /**
   * @brief Handles incoming deprecations.
   * @param entity                    Entity to deprecate.
   * @param detail                    Deprecation detail.
   */
  void deprecated(Entity *entity, const Syntax::Node *node);

  /**
   * @brief Handles preamble checking.
   * @param preamble                  Preamble to check.
   * @param entity                    Bound entity reference.
   */
  Entity *preamble() const noexcept;
  Deduction preamble(const Syntax::Preamble *preamble, Entity *entity);

  /**
   * @brief Constructs an unreachable control-flow.
   * @param degree                    Reachability degree.
   * @param effect                    Control flow effect.
   */
  Deduction unreachable(Flow::Effect effect = Flow::Effect::NONE) const;
  Deduction unreachable(Flow::Degree degree, Flow::Effect effect = Flow::Effect::NONE) const;

  /**
   * @brief Handles checking individual nodes.
   * @param node                      Node to check.
   * @param fallback                  Fallback type.
   */
  Deduction check(const Syntax::Node *node, const Erased &fallback = New::unset());

  /**
   * @brief Checks a sequence of nodes.
   * @param nodes                     Nodes to check.
   */
  Deduction check(const std::vector<Syntax::Node *> &nodes);
  void check(const std::vector<Syntax::Expression *> &nodes);

  /**
   * @brief Handles instantiating a parameter list.
   * @param parameters                Parameters to instantiate.
   */
  Template check(const Syntax::Template &parameters);

  /**
   * @brief Handles running a complete type-audit.
   * @param tree                      Syntax tree node.
   * @param reporter                  Diagnostic reporter.
   */
  $::Unique::Pointer<Exports> audit(const Syntax::Tree *tree, Diagnostic::Reporter *reporter = nullptr);

  /**
   * @brief Handles instantiating
   * @param type                      Type to instantiate
   * @param arguments                 Generic arguments.
   */
  Erased instantiate(const Erased &type, const Syntax::Specialization &arguments);

  /**
   * @brief Handles merging flows together.
   * @param left                      Left-most flow.
   * @param right                     Right-most flow.
   */
  Control merge(Control &&left, Control &&right) const noexcept;

  /**
   * @brief Handles branching decisions.
   * @param deduction                 Deduction details.
   * @param left                      Left-branch handler.
   * @param right                     Optional right-branch.
   */
  Deduction branch(Deduction &&deduction, Branch &&left, Branch &&right);

  /**
   * @brief Handles importing resources.
   * @param path                      Import resource.
   * @param range                     Optional range.
   */
  const Exports *import(const $::String::View &path, const XLSP::Range &range = {});
  const Exports *import(const $::URI::Buffer &resource, const XLSP::Range &range = {});

  /**
   * @brief Reports a diagnostic.
   * @param code                      Diagnostic code.
   * @param args                      Message arguments.
   */
  template <class... As> inline constexpr Deduction report(Diagnostic::Code code, As &&...args) {
    return report(XLSP::Range(), code, std::forward<As>(args)...);
  }

  /**
   * @brief Reports a diagnostic.
   * @param range                     Resource range.
   * @param code                      Diagnostic code.
   * @param args                      Message arguments.
   */
  template <class... As>
  inline constexpr Deduction report(const XLSP::Range &range, Diagnostic::Code code, As &&...args) {
    if (m_reporter) m_reporter->emit(range, code, std::forward<As>(args)...);
    return passable(New::fail()); // bind a failure typing to be used
  }

  /**
   * @brief Reports a diagnostic.
   * @param node                      Syntax node.
   * @param code                      Diagnostic code.
   * @param args                      Message arguments.
   */
  template <std::derived_from<Syntax::Node> T, class... As>
  inline constexpr Deduction report(const T *node, Diagnostic::Code code, As &&...args) {
    return report(node->trivia()->range(), code, std::forward<As>(args)...);
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles making deductions from a branch.
   * @param branch                    Branch to execute.
   */
  Control m_using(Branch &&branch);
};

} // namespace Sabre::Type

#endif
