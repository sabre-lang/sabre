#ifndef _SABRE_RELINT_EXPORTS_HPP
#define _SABRE_RELINT_EXPORTS_HPP

/// Sabre Includes
#include "sabre/relint/scope.hpp"
#include "sabre/syntax/comparator.hpp"

namespace Sabre::Relint {

/// @brief Document Symbols Alias.
using Symbols = std::vector<XLSP::Symbol>;

/// @brief Function type for search filtering.
using Filter = $::Shared::Functor<bool(const Syntax::Node *) const>;

/// @brief Relint Environment Metadata.
class Exports {
  //  TYPEDEFS  //

  /// @brief Allow the linter internal access.
  friend struct Analyzer;

  //  PROPERTIES  //

  /// @brief The available document symbols.
  Symbols m_symbols = {};

  /// @brief The sorted list of mirrors.
  std::vector<const Syntax::Node *> m_sorted = {};

  /// @brief Prepare the base references scope.
  $::Unique::Pointer<Scope> m_references = $::Unique::New<Scope>();

  /// @brief Mirrors that contain node reflection data.
  $::Map::Base<const Syntax::Node *, $::Unique::Pointer<Mirror>> m_storage = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs linting exports.
  explicit Exports() = default;

  //  PUBLIC METHODS  //

  /// @brief Gets the available document symbols.
  inline constexpr const Symbols &symbols() const noexcept { return m_symbols; }

  /// @brief Gets the core references scoping.
  inline constexpr const Scope *references() const noexcept { return m_references.get(); }

  /**
   * @brief Denotes if a mirror is bound.
   * @param origin                Origin node.
   */
  bool bound(const Syntax::Node *origin) const;

  /**
   * @brief Handles resolving syntax mirrors from a node.
   * @param origin                Origin node.
   */
  const Mirror *resolve(const Syntax::Node *origin) const;

  /**
   * @brief Handles resolving syntax mirrors from a node.
   * @param origin                Origin node.
   * @param parent                Parent node.
   */
  Mirror *resolve(const Syntax::Node *origin, const Syntax::Node *parent = nullptr);

  /**
   * @brief Handles searching for a node mirror.
   * @param position              Search position.
   * @param filter                Optional filter.
   */
  const Mirror *search(const XLSP::Position &position) const noexcept;
  const Mirror *search(const XLSP::Position &position, Filter &&filter) const noexcept;

  /// @brief Handles filtering metadata.
  template <std::derived_from<Syntax::Node> T> inline constexpr std::vector<const T *> filter() const noexcept {
    // prepare all the validly filtered items
    std::vector<const T *> filtered = {};

    // attempt filtering now
    for (const auto *node : m_sorted) {
      if (node->is<T>()) filtered.emplace_back(node->as<T>());
    }

    // return the resulting nodes now
    return filtered;
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles constructing a mirror.
   * @param origin                Origin node.
   * @param parent                Parent node.
   */
  $::Unique::Pointer<Mirror> m_create(const Syntax::Node *origin, const Syntax::Node *parent) const noexcept;
};

} // namespace Sabre::Relint

#endif
