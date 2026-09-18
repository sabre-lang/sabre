#ifndef _SABRE_SYNTAX_TRIVIA_HPP
#define _SABRE_SYNTAX_TRIVIA_HPP

/// Sabre Includes
#include "sabre/forward/type.hpp"
#include "sabre/syntax/bounds.hpp"

namespace Sabre::Syntax {

/// @brief Node Trivia.
class Trivia {
  //  TYPEDEFS  //

  /// @brief Allow some items internal access.
  friend class Node;
  friend class Tree;

  //  PROPERTIES  //

  /// @brief Encompassing resource bounds.
  Bounds m_bounds = Bounds();

  /// @brief Resultant type from analysis.
  Type::Erased m_type = nullptr;

  /// @brief The underlying hashed node.
  const Node *m_keyid = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Allow default construction of invalid node trivia.
  constexpr Trivia() = default;

  /**
   * @brief Constructs node trivia.
   * @param keyid           Node keyid.
   * @param bounds          Node bounds.
   */
  template <std::derived_from<Node> T>
  constexpr Trivia(const T *keyid, const Bounds &bounds = Bounds()) : m_bounds(bounds), m_keyid(keyid) {}

  //  PUBLIC METHODS  //

  /// @brief The associated hashed node tag.
  inline constexpr $::RTTI::Tag hash() const noexcept { return m_hash(); }

  /// @brief Gets the nodes available ranges.
  inline constexpr const Bounds &bounds() const noexcept { return m_bounds; }

  /// @brief Helper for getting the nodes innermost range.
  inline constexpr XLSP::Range range() const noexcept { return m_bounds.inner(); }

  /// @brief Gets the debug-name of a given node.
  inline constexpr $::String::View name() const noexcept { return m_name(); }

  /// @brief Gets the associated node type.
  inline constexpr Type::Erased &type() noexcept { return m_type; }
  inline constexpr const Type::Erased &type() const noexcept { return m_type; }

  /// @brief Gets the current truthiness.
  inline constexpr $::Unit::Ternary truthiness() const noexcept { return m_truthiness(); }

private:
  //  PRIVATE METHODS  //

  /// @brief Gets the bound runtime hash.
  $::RTTI::Tag m_hash() const noexcept;

  /// @brief Gets the associated node name.
  $::String::View m_name() const noexcept;

  /// @brief Gets the underlying truthiness.
  $::Unit::Ternary m_truthiness() const noexcept;
};

} // namespace Sabre::Syntax

#endif
