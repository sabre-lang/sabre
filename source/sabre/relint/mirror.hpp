#ifndef _SABRE_RELINT_MIRROR_HPP
#define _SABRE_RELINT_MIRROR_HPP

/// Sabre Includes
#include "sabre/relint/comments.hpp"
#include "sabre/syntax/visitor.hpp"

namespace Sabre::Relint {

/// @brief Mirror Reference Definition.
struct Definition {
  Mirror *variable = nullptr;   // Definition variable value.
  Mirror *annotation = nullptr; // Definition annotation value.
};

/// @brief Helper for relinting overloads.
template <class... Ts> struct Overloads : Ts... {
  //  TYPEDEFS  //

  /// @brief Allow access to operators.
  using Ts::operator()...;

  //  PROPERTIES  //

  /// @brief Denotes if particular methods can be invoked.
  template <class... As> static constexpr bool invocable = std::is_invocable<Overloads<Ts...>, As...>();

  /// @brief Denotes if a node has partial support.
  template <std::derived_from<Syntax::Node> T> static constexpr bool partial = invocable<const T *>;

  /// @brief Denotes if a node has full support.
  template <std::derived_from<Syntax::Node> T> static constexpr bool full = invocable<const T *, const Mirror *>;
};

/// @brief Encapsulates Reflected Syntax Metadata.
class Mirror {
  //  PROPERTIES  //

  /// @brief The associated parent node.
  const Mirror *m_parent = nullptr;

  /// @brief The associated origin node.
  const Syntax::Node *m_origin = nullptr;

  /// @brief The associated definition (type and/or value).
  $::Shared::Pointer<Definition> m_definition = $::Shared::New<Definition>();

  /// @brief Currently available references to the origin node.
  std::vector<const Mirror *> m_references = {};

  /// @brief Currently available fields for the mirror.
  $::Map::Dict<$::Shared::Pointer<Definition>> m_fields = {};

public:
  //  CONSTRUCTORS  //

  /// @brief An anonymous mirror instance.
  constexpr Mirror() = default;

  /**
   * @brief Constructs a syntax mirror.
   * @param origin                Origin node.
   * @param parent                Parent node.
   */
  constexpr Mirror(const Syntax::Node *origin, const Mirror *parent = nullptr) : m_parent(parent), m_origin(origin) {}

  //  PUBLIC METHODS  //

  /// @brief Parent node of the mirror.
  inline constexpr const Mirror *parent() const noexcept { return m_parent; }

  /// @brief The origin of the mirror.
  inline constexpr const Syntax::Node *origin() const noexcept { return m_origin; }

  /// @brief Gets the underlying typing.
  inline constexpr Type::Erased type() const noexcept { return m_origin->trivia()->type(); }

  /// @brief Gets the comments for a mirror.
  inline constexpr Comments comments() const noexcept { return Comments(this); }

  /// @brief Gets the associated range/bounds values.
  inline constexpr XLSP::Range range() const noexcept { return m_origin->trivia()->range(); }
  inline constexpr const Syntax::Bounds &bounds() const noexcept { return m_origin->trivia()->bounds(); }

  /// @brief Variable definition of the node.
  inline constexpr Definition *definition() noexcept { return m_definition.get(); }
  inline constexpr const Definition *definition() const noexcept { return m_definition.get(); }

  /// @brief All available references for the node.
  inline constexpr std::vector<const Mirror *> &references() noexcept { return m_references; }
  inline constexpr const std::vector<const Mirror *> &references() const noexcept { return m_references; }

  /// @brief All available fields for the node.
  inline constexpr auto &fields() noexcept { return m_fields; }
  inline constexpr const auto &fields() const noexcept { return m_fields; }

  /// @brief Gets the canonical representation of the node.
  inline constexpr $::String::View canonical() const noexcept { return m_origin->trivia()->name(); }

  /// @brief Checks if a node is type-qualified or not (eg: type-world identifier or value identifier).
  inline constexpr bool qualified() const noexcept { return filter<Syntax::Qualifier>() || filter<Syntax::Alias>(); }

  /// @brief Allow conversion to any valid typing.
  template <std::derived_from<Syntax::Node> T> inline constexpr const T *when() const noexcept {
    return m_origin->is<T>() ? static_cast<const T *>(m_origin) : nullptr;
  }

  /// @brief Attempts scanning upwards for a parent.
  template <std::derived_from<Syntax::Node> T> inline constexpr const T *parent() const noexcept {
    if (m_parent == nullptr) return nullptr;
    if (auto node = m_parent->when<T>()) return node;
    return m_parent->parent<T>(); // scan further
  }

  /// @brief Attempts scanning upwards from this node.
  template <std::derived_from<Syntax::Node> T> inline constexpr const T *filter() const noexcept {
    if (auto node = when<T>()) return node;
    else return parent<T>(); // scan upwards
  }

  /**
   * @brief Handles looking up field definitions.
   * @param name                  Name of member field.
   */
  inline constexpr $::Shared::Pointer<Definition> lookup(const $::String::View &name) {
    auto iter = m_fields.find($::String::Buffer(name)); // attempt finding field
    return iter == m_fields.cend() ? $::Shared::New<Definition>() : iter->second;
  }

  /// @brief Allows visiting different nodes.
  template <class... Fs> inline constexpr auto visit(Overloads<Fs...> &&overloads) const noexcept {
#define X(N, ...)                                                       \
  case $::RTTI::Hash<Syntax::N>(): {                                    \
    if constexpr (Overloads<Fs...>::template full<Syntax::N>)           \
      return overloads(static_cast<const Syntax::N *>(m_origin), this); \
    else if constexpr (Overloads<Fs...>::template partial<Syntax::N>)   \
      return overloads(static_cast<const Syntax::N *>(m_origin));       \
  } break;

    // attempt handling the incoming node instance now
    switch (m_origin->trivia()->hash()) { SABRE_XX_SYNTAX_NODES(X) default : break; }
#undef X

    if constexpr (Overloads<Fs...>::template invocable<>) return overloads();
    else if constexpr (Overloads<Fs...>::template invocable<const Mirror *>) return overloads(this);
    else if constexpr (Overloads<Fs...>::template partial<Syntax::Node>) return overloads(m_origin);
    else if constexpr (Overloads<Fs...>::template full<Syntax::Node>) return overloads(m_origin, this);
  }
};

} // namespace Sabre::Relint

#endif
