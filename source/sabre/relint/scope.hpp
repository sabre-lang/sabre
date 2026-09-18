#ifndef _SABRE_RELINT_SCOPE_HPP
#define _SABRE_RELINT_SCOPE_HPP

/// Sabre Includes
#include "sabre/relint/mirror.hpp"

namespace Sabre::Relint {

/// @brief References Scoping.
class Scope {
  //  TYPEDEFS  //

  /// @brief Allow the base analyzer internal access.
  friend struct Analyzer;

  //  PROPERTIES  //

  /// @brief Previous references scope.
  Scope *m_ancestor = nullptr;

  /// @brief All bound variable definitions.
  $::Map::Dict<$::Shared::Pointer<Definition>> m_definitions = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a references scope.
   * @param ancestor          Scope ancestor.
   */
  explicit Scope(Scope *ancestor = nullptr) : m_ancestor(ancestor) {}

  //  PUBLIC METHODS  //

  /// @brief Gets a view of the available definitions.
  inline constexpr auto view() const noexcept { return m_definitions; }

  /**
   * @brief Allows overwriting an immediate definition.
   * @param name              Name of variable.
   * @param definition        Definition to bind.
   */
  inline void overwrite(const $::String::View &name, const $::Shared::Pointer<Definition> &definition) noexcept {
    m_definitions.try_emplace($::String::Buffer(name), definition); // we immediately update here
  }

  /**
   * @brief Declares a variable reference.
   * @param name              Name of variable.
   * @param mirror            Mirror to bind.
   */
  inline constexpr bool declare(const $::String::View &name, Mirror *mirror) noexcept {
    return m_define(name, mirror, [](auto *definition) -> Mirror *& { return definition->variable; });
  }

  /**
   * @brief Declares an annotation reference.
   * @param name              Name of annotation.
   * @param mirror            Mirror to bind.
   */
  inline constexpr bool annotate(const $::String::View &name, Mirror *mirror) noexcept {
    return m_define(name, mirror, [](auto *definition) -> Mirror *& { return definition->annotation; });
  }

  /**
   * @brief Resolves a definition reference.
   * @param name              Name of variable.
   */
  inline constexpr $::Shared::Pointer<Definition> resolve(const $::String::View &name) const noexcept {
    auto iter = m_definitions.find($::String::Buffer(name));
    if (iter != m_definitions.cend()) return iter->second;
    return m_ancestor ? m_ancestor->resolve(name) : $::Shared::New<Definition>();
  }

  /**
   * @brief We allow linking relational references.
   * @param name              Name of definition.
   * @param child             Child mirror to bind.
   */
  inline void relate(const $::String::View &name, Mirror *child) const { return relate(child, resolve(name)); }
  inline void relate(Mirror *child, const $::Shared::Pointer<Definition> &parent) const {
    capture(child, parent), subtype(child, parent);
  }

  /**
   * @brief We allow linking variable references.
   * @param name              Name of definition.
   * @param child             Child mirror to bind.
   */
  inline void capture(const $::String::View &name, Mirror *child) const { return capture(child, resolve(name)); }
  inline void capture(Mirror *child, const $::Shared::Pointer<Definition> &parent) const {
    if (!parent || !parent->variable) return;
    parent->variable->references().emplace_back(child);
    child->definition()->variable = parent->variable;
  }

  /**
   * @brief We allow linking annotation references.
   * @param name              Name of definition.
   * @param child             Child mirror to bind.
   */
  inline void subtype(const $::String::View &name, Mirror *child) const { return subtype(child, resolve(name)); }
  inline void subtype(Mirror *child, const $::Shared::Pointer<Definition> &parent) const {
    if (!parent || !parent->annotation) return;
    parent->annotation->references().emplace_back(child);
    child->definition()->annotation = parent->annotation;
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Declares a reference instance.
   * @param name              Name of variable.
   * @param mirror            Mirror to define.
   * @param resolver          Resolver function.
   */
  template <class R>
  inline constexpr bool m_define(const $::String::View &name, Mirror *mirror, R &&resolver) noexcept {
    return m_define($::String::Buffer(name), mirror, std::move(resolver));
  }

  /**
   * @brief Declares a reference instance.
   * @param name              Name of variable.
   * @param mirror            Mirror to define.
   * @param resolver          Resolver function.
   */
  template <class R>
  inline constexpr bool m_define(const $::String::Buffer &name, Mirror *mirror, R &&resolver) noexcept {
    // check if the definition actually exists at all
    if (!m_definitions.contains(name)) m_definitions.try_emplace(name, $::Shared::New<Definition>());

    // get the current definition instance
    auto &definition = m_definitions.at(name);
    auto &reference = resolver(definition.get());

    // check if we had a valid definition
    auto updated = reference == nullptr;

    // if the reference is defined, then ignore
    if (updated) resolver(mirror->definition()) = reference = mirror;

    // declare as a success now
    return updated;
  }
};

} // namespace Sabre::Relint

#endif
