#ifndef _SABRE_AUTHOR_JSON_IPP
#define _SABRE_AUTHOR_JSON_IPP

/// Sabre Includes
#include "sabre/author/serializer.hpp"
#include "sabre/type/mixin.hpp"

namespace Sabre::Author {

/// @brief JSON Reporter Implementation.
template <> class Serializer::Proxy<Format::JSON> : public Serializer::Abstract {
  //  PROPERTIES  //

  /// @brief Prepare all the nodes to encode.
  $::Serde::Object m_nodes = {};

  /// @brief Prepare the groups to encode.
  $::Map::Dict<$::Serde::Array> m_groups = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Abstract::Abstract;

protected:
  //  PRIVATE METHODS  //

  /// @brief Gets the label of the proxy.
  inline constexpr $::String::View m_label() const noexcept final { return "JSON"; }

  /// @brief Gets the output extension to be used.
  inline constexpr $::String::View m_extension() const noexcept final { return ".jsonc"; }

  /**
   * @brief Resolves a key for a node.
   * @param node                Node to resolve.
   */
  inline constexpr $::Serde::Text m_key(const Syntax::Node *node) const noexcept {
    return fmt::to_string(reinterpret_cast<uintptr_t>(node));
  }

  /**
   * @brief Handles binding group values.
   * @param node                Node to bind.
   * @param category            Category value.
   * @param plural              Plural suffix.
   */
  inline constexpr $::String::Buffer
  m_bind(const Syntax::Node *node, const $::String::Buffer &category, const $::String::Buffer &plural = "s") noexcept {
    // prepare the incoming title to be using now
    auto title = $::Convert::capitalize(category) + plural;

    // if we do not have a suitable iterator, then lets create one
    if (!m_groups.contains(title)) m_groups.emplace(title, $::Serde::Array());

    // finally append the current key now and return the category value
    return m_groups.at(title).emplace_back(m_key(node)), category;
  }

  /**
   * @brief Resolves a variant for a node.
   * @param node                Node to resolve.
   */
  inline constexpr $::Serde::Value m_variant(const Syntax::Declaration *node) noexcept {
    // prepare a set of overloads for resolving the variant now
    switch (node->trivia()->hash()) {
    case $::RTTI::Hash<Syntax::Enum>(): return m_bind(node, "enum");
    case $::RTTI::Hash<Syntax::Alias>(): return m_bind(node, "typedef");
    case $::RTTI::Hash<Syntax::Class>(): return m_bind(node, "class", "es");
    case $::RTTI::Hash<Syntax::Variable>(): return m_bind(node, "variable");
    case $::RTTI::Hash<Syntax::Namespace>(): return m_bind(node, "namespace");

    // failed to match a suitable value here
    default: return $::Serde::Null();
    }
  }

  /** Handles rendering the final result. */
  inline constexpr $::String::Buffer m_render() noexcept final {
    // iterably append all the available nodes now
    for (const auto &[node, declaration] : project()->storage) {
      // get the underlying mirror instance
      auto *mirror = declaration->mirror();

      // prepare the encoded output
      $::Serde::Object encoded = {
          {"name", node->name()},
          {"description", mirror->comments().join()},
          {"typing", fmt::to_string(*mirror->type())},
      };

      // resolve the variant now as necessary
      if (auto variant = m_variant(node)) encoded["variant"] = variant;

      // emplace the current node as necessary now
      m_nodes.emplace(m_key(node), encoded);
    }

    // prepare our groups to be emplaced
    auto groups = $::Serde::Array();
    groups.reserve(m_groups.size());

    // we need to convert our groups into a suitable set of outputs
    for (const auto &[title, children] : m_groups) {
      groups.emplace_back($::Serde::Object{{"title", title}, {"children", children}});
    }

    // prepare the output object to be used
    $::Serde::Object output = {
        {"nodes", m_nodes},
        {"groups", groups},
        {"title", project()->title},
        {"description", project()->description},
    };

    // and encode the result as needed for serialization
    return $::JSON::Encode(output).value();
  }
};

} // namespace Sabre::Author

#endif
