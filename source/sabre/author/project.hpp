#ifndef _SABRE_AUTHOR_PROJECT_HPP
#define _SABRE_AUTHOR_PROJECT_HPP

/// Sabre Includes
#include "sabre/forward/author.hpp"
#include "sabre/relint/mirror.hpp"

namespace Sabre::Author {

/// @brief Project Declaration Node.
class Declaration {
  //  PROPERTIES  //

  /// @brief Associated declaration node.
  const Syntax::Declaration *m_node = nullptr;

  /// @brief Associated declaration mirror.
  const Relint::Mirror *m_mirror = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Do not allow default construction.
  constexpr Declaration() = delete;

  /**
   * @brief Constructs a declaration reference.
   * @param mirror              Mirror to bind.
   */
  constexpr Declaration(const Relint::Mirror *mirror) : Declaration(mirror->when<Syntax::Declaration>(), mirror) {}

  /**
   * @brief Constructs a declaration reference.
   * @param node                Declaration node.
   * @param mirror              Mirror to bind.
   */
  constexpr Declaration(const Syntax::Declaration *node, const Relint::Mirror *mirror) :
      m_node(node), m_mirror(mirror) {}

  //  PUBLIC METHODS  //

  inline constexpr const Syntax::Declaration *node() const noexcept { return m_node; }
  inline constexpr const Relint::Mirror *mirror() const noexcept { return m_mirror; }
};

/// @brief Storage Alias for Projects.
using Storage = $::Map::Base<const Syntax::Declaration *, $::Unique::Pointer<Declaration>>;

/// @brief Core Authoring Project.
struct Project {
  //  PROPERTIES  //

  /// @brief Base project resource.
  $::URI::View resource = {};

  /// @brief Project documentation title.
  $::String::Buffer title = {};

  /// @brief Project documentation description.
  $::String::Buffer description = {};

  /// @brief Storage of all available declarations.
  Storage storage = {};
};

} // namespace Sabre::Author

#endif
