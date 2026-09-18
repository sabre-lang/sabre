#ifndef _SABRE_VARIABLE_ANALYZER_HPP
#define _SABRE_VARIABLE_ANALYZER_HPP

/// Sabre Incldues
#include "sabre/syntax/tree.hpp"
#include "sabre/variable/captures.hpp"

namespace Sabre::Variable {

/// @brief Variable Depth Alias.
using Depth = int32_t;

/// @brief Variable Analyzer Instance.
class Analyzer {
  //  TYPEDEFS  //

  /// @brief Entries Mapping for Captures.
  using Entries = $::Map::Record<Entry>;

  /// @brief Deferred Handler Typing.
  using Deferrer = $::Shared::Functor<void(Analyzer *) const>;

  /// @brief Resultant Lookup Typing.
  using Lookup = std::pair<Variable::Entry, Depth>;

  //  PROPERTIES  //

  /// @brief The current outer scoping.
  Depth m_outer = 0;

  /// @brief The captures to be analyzed.
  Variable::Captures *m_captures = nullptr;

  /// @brief Currently available entries bound.
  std::vector<Entries> m_scopes = {Entries()};

  /// @brief Currently queued items for visiting.
  std::vector<Deferrer> m_deferred = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a defaulted analyzer.
  constexpr Analyzer() = default;

  /// @brief Handles closing the final scoping.
  constexpr ~Analyzer() { m_close(); }

  //  PUBLIC METHODS  //

  /// @brief Gets the current parser scope depth.
  inline constexpr Depth depth() const noexcept { return m_scopes.size(); }

  /// @brief Handles setting a new scoping.
  inline constexpr auto scope() noexcept {
    m_scopes.emplace_back(Entries());
    return $::Lambda::Defer([&] { m_close(); });
  }

  /// @brief Handles setting a new scoping with outer depth.
  inline constexpr auto scope(Depth depth) noexcept {
    std::swap(m_outer, depth), m_scopes.emplace_back(Entries());
    return $::Lambda::Defer([&, depth] { m_close(depth); });
  }

  /// @brief Handles setting a new scoping and declaration.
  template <std::derived_from<Syntax::Node> T> inline constexpr auto scope(const T *node) noexcept {
    return bind(node), scope();
  }

  /// @brief Handles setting a new scoping with outer depth and declaration.
  template <std::derived_from<Syntax::Node> T> inline constexpr auto scope(const T *node, Depth depth) noexcept {
    return bind(node), scope(depth);
  }

  /**
   * @brief Handles visiting incoming nodes.
   * @param node                  Node to visit.
   */
  inline constexpr void visit(const Syntax::Node *node) { m_visit(node); }
  inline constexpr void visit(Syntax::Tree *tree) { m_reset(tree), m_visit(tree); }

  /**
   * @brief Handles visiting incoming nodes.
   * @param nodes                 Nodes to visit.
   */
  template <std::derived_from<Syntax::Node> T> inline constexpr void visit(const std::vector<T *> &nodes) {
    for (const auto *node : nodes) m_visit(node);
  }

  /**
   * @brief Handles visiting premable nodes.
   * @param node                  Node to visit.
   */
  inline constexpr void preamble(const Syntax::Preamble *node) {
    visit(node->attributes());
    visit(node->decorators());
  }

  /**
   * @brief Handles deferring type-checking.
   * @param deduction             Passthrough result.
   * @param deferrer              Deferred handler.
   */
  inline constexpr void defer(Deferrer &&deferrer) { m_deferred.emplace_back(std::move(deferrer)); }

  /**
   * @brief Handles binding captures.
   * @param node                  Node to bind.
   */
  template <std::derived_from<Syntax::Node> T> inline constexpr void bind(const T *node) noexcept {
    m_scopes.back().emplace(node->name(), m_captures->declare(node));
  }

  /**
   * @brief Handles binding captures.
   * @param nodes                 Nodes to bind.
   */
  template <std::derived_from<Syntax::Node> T> inline constexpr void bind(const std::vector<T *> &nodes) noexcept {
    for (const auto &node : nodes) bind(node);
  }

  /**
   * @brief Handles safely marking identifier.
   * @param identifier            Identifier to mark.
   */
  inline constexpr void mark(const Syntax::Identifier *identifier) {
    // if the identifier does not exist, then ignore
    if ($_UNLIKELY(identifier == nullptr)) return;

    // attempt resolving a suitable declaration and depth
    auto [entry, depth] = m_lookup(identifier->name());

    // check if our item in question is a potential global
    if (depth < 0) return m_captures->global(identifier);

    // otherwise we check our candidates against the current outer scope
    if (depth >= m_outer) return m_captures->mark(identifier, entry);

    // if we reached here, then we have a variable that has leaked
    m_captures->leak(identifier, entry);
  }

private:
  //  PRIVATE METHODS  //

  /// @brief Handles closing the current scoping.
  void m_close();
  void m_close(Depth depth);

  /**
   * @brief Handles resetting the analyzer.
   * @param tree                  Tree to use.
   */
  void m_reset(Syntax::Tree *tree);

  /**
   * @brief Handles visiting incoming nodes.
   * @param node                  Node to visit.
   */
  void m_visit(const Syntax::Node *node);

  /**
   * @brief Handles finding a declaration for an identifier.
   * @param name                  Identifier to query.
   */
  Lookup m_lookup(const $::String::View &name) noexcept;
};

} // namespace Sabre::Variable

#endif
