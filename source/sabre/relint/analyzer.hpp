#ifndef _SABRE_RELINT_ANALYZER_HPP
#define _SABRE_RELINT_ANALYZER_HPP

/// Sabre Includes
#include "sabre/diagnostic/scope.hpp"
#include "sabre/relint/context.hpp"
#include "sabre/relint/options.hpp"
#include "sabre/syntax/node.hpp"

namespace Sabre::Relint {

/// @brief Linter Analyzer.
struct Analyzer : public XI::Transient, public Context {
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an analyzer instance.
   * @param services                  Services container.
   */
  explicit Analyzer();
  explicit Analyzer(XI::Container *services);

  //  PUBLIC METHODS  //

  /// @brief Gets the current scoped references.
  inline constexpr Scope *references() noexcept { return m_scope; }

  /// @brief Current analyzer metadata.
  inline constexpr Exports *mirrors() noexcept { return m_mirrors.get(); }

  /// @brief Handles scoping deferred callbacks.
  inline constexpr auto scope() {
    // prepare a lambda for generating a reference
    auto bind = [&] {
      auto scope = $::Unique::New<Scope>(m_scope);
      return m_scope = scope.get(), std::move(scope);
    };

    // and defer destruction handling now
    return $::Lambda::Defer([&, scope = bind()] { m_scope = scope->m_ancestor; });
  }

  /// @brief Handles scoping symbols.
  inline constexpr auto symbol(const Syntax::Declaration *node, XLSP::Symbol::Kind kind) {
    m_symbol = m_bind(XLSP::Symbol(node->name(), kind), node);
    return $::Lambda::Defer([&] { m_symbol = nullptr; });
  }

  /// @brief Handles scoping symbols.
  template <std::derived_from<Syntax::Node> T>
  inline constexpr auto symbol(const $::String::View &name, const T *node, XLSP::Symbol::Kind kind) {
    // handle the result to be returned now
    auto *symbol = m_bind(XLSP::Symbol(name, kind), node);

    // determine whether or not we have a scopable item
    static constexpr auto s_scoped = std::same_as<T, Syntax::Lambda>;

    if constexpr (!s_scoped) return; // ignore anything that is not scoped here
    else return m_symbol = symbol, $::Lambda::Defer([&] { m_symbol = nullptr; });
  }

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

  /**
   * @brief Handles importing module references.
   * @param path                      Resource to import.
   */
  const Scope *import(const $::String::View &path);
  const Scope *import(const $::URI::Buffer &resource);

  /**
   * @brief Handles running a complete linter-audit.
   * @param tree                      Syntax tree node.
   * @param reporter                  Diagnostic reporter.
   */
  $::Unique::Pointer<Exports> audit(const Syntax::Tree *tree, Diagnostic::Reporter *reporter);

  /**
   * @brief Handles linting a singular node.
   * @param node                      Node to lint.
   * @param parent                    Parent to bind.
   * @param visit                     Visit children.
   */
  Mirror *verify(const Syntax::Node *node, const Syntax::Node *parent = nullptr, bool visit = true);

  /**
   * @brief Handles linting a multiple nodes.
   * @param nodes                     Nodes to lint.
   * @param parent                    Parent to bind.
   * @param visit                     Visit children.
   */
  template <std::derived_from<Syntax::Node> T>
  inline constexpr std::vector<Mirror *> verify(const std::vector<T *> &nodes, const Syntax::Node *parent = nullptr) {
    auto predicate = [&](const T *node) { return verify(node, parent); };
    return $::Ranges::To(nodes | std::views::transform(predicate));
  }

  /**
   * @brief Handles traversing a node.
   * @param node                      Node to traverse.
   */
  void traverse(const Syntax::Node *node);

  /**
   * @brief Handles traversing a multiple nodes.
   * @param nodes                     Nodes to lint.
   */
  template <std::derived_from<Syntax::Node> T> inline constexpr void traverse(const std::vector<T *> &nodes) {
    for (const auto *node : nodes) traverse(node);
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles finalizing mirror data (eg: comments, ...).
   * @param mirror                    Mirror to finalize.
   */
  void m_finalize(Mirror *mirror);

  /**
   * @brief Handles constructing symbols.
   * @param symbol                    Symbol to bind
   * @param node                      Node of symbol.
   */
  XLSP::Symbol *m_bind(XLSP::Symbol &&symbol, const Syntax::Node *node);
};

} // namespace Sabre::Relint

#endif
