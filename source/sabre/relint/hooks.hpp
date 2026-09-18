#ifndef _SABRE_RELINT_HOOKS_HPP
#define _SABRE_RELINT_HOOKS_HPP

/// Sabre Includes
#include "sabre/relint/options.hpp"
#include "sabre/relint/rule.hpp"
#include "sabre/syntax/node.hpp"

namespace Sabre::Relint {

/// @brief Handles Delgating Hooks.
template <std::derived_from<Syntax::Node> T> using Delegate = $::Shared::Functor<void(const T *, Context *) const>;

/// @brief Rule Hooks Container.
class Hooks {
  //  TYPEDEFS  //

  /// @brief Allow analyzers internal access.
  friend struct Analyzer;

  /// @brief Generic Delegate Typing.
  using Generic = Delegate<Syntax::Node>;

  //  PROPERTIES  //

  /// @brief The currently bound options.
  Options m_options = {};

  /// @brief The available delegates to be used.
  $::Map::Base<$::RTTI::Tag, std::vector<Generic>> m_delegates = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a set of hooks.
   * @param options               Bound options.
   */
  explicit Hooks(const Options &options = {}) : m_options(options) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the available options for the hooks.
  inline constexpr const Options *options() const noexcept { return &m_options; }

  /**
   * @brief Handles a rule for an incoming node.
   * @param delegate              Hook delegate.
   */
  template <std::derived_from<Syntax::Node> T> inline constexpr Hooks *overload(Delegate<T> &&delegate) {
    auto generic = m_generic(std::move(delegate)); // cast
    return m_bind($::RTTI::Hash<T>(), std::move(generic));
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles resolving delegates.
   * @param node                  Syntax node.
   */
  inline constexpr const std::vector<Generic> &m_resolve(const Syntax::Node *node) {
    return m_resolve(node->trivia()->hash());
  }

  /**
   * @brief Handles resolving delegates.
   * @param tag                   Node tag.
   */
  inline constexpr std::vector<Generic> &m_resolve($::RTTI::Tag tag) {
    return m_delegates.try_emplace(tag, std::vector<Generic>()).first->second;
  }

  /**
   * @brief Handles binding a delegate.
   * @param tag                   Node tag.
   * @param delegate              Hook delegate.
   */
  inline constexpr Hooks *m_bind($::RTTI::Tag tag, Delegate<Syntax::Node> &&delegate) {
    return m_resolve(tag).emplace_back(std::move(delegate)), this;
  }

  /**
   * @brief Handles generic delegate casts.
   * @param delegate              Hook delegate.
   */
  template <std::derived_from<Syntax::Node> T>
  inline constexpr Generic m_generic(Delegate<T> &&delegate) const noexcept {
    return [cb = std::move(delegate)](const Syntax::Node *node, Context *context) { cb(node->as<T>(), context); };
  }
};

} // namespace Sabre::Relint

#endif
