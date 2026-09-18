#ifndef _SABRE_SERVER_UTILITIES_HPP
#define _SABRE_SERVER_UTILITIES_HPP

/// Sabre Include
#include "sabre/forward/server.hpp"
#include "sabre/relint/exports.hpp"

namespace Sabre::Server {

/// @brief Server Utilities Class.
class Utilities : public XI::Transient {
  //  PROPERTIES  //

  /// @brief Bound container of services.
  XI::Container *m_services;

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs the underlying utilities.
  explicit Utilities();
  explicit Utilities(XI::Container *services);

  //  PUBLIC METHODS  //

  /**
   * @brief Converts syntax to a location.
   * @param node                      Node to be converted.
   * @param resource                  Resource to bind
   */
  XLSP::Reference syntax_to_client(const Relint::Mirror *mirror) const;
  XLSP::Reference syntax_to_client(const Syntax::Node *node, const $::URI::Buffer &resource = {}) const;

  /**
   * @brief Gets the linter metadata at a resource.
   * @param resource                  Resource value.
   */
  const Relint::Exports *syntax_view_at(const $::URI::View &resource) const;

  /**
   * @brief Gets a syntax-node at a location.
   * @param resource                  Resource value.
   * @param position                  Position value.
   * @param filter                    Optional filter.
   */
  const Relint::Mirror *
  syntax_node_at(const $::URI::View &resource, const XLSP::Position &position, Relint::Filter &&filter = nullptr) const;

  /**
   * @brief Gets a syntax-node at a location.
   * @param resource                  Resource value.
   * @param position                  Position value.
   */
  template <std::derived_from<Syntax::Node>... Ts>
  inline constexpr const Relint::Mirror *
  syntax_node_at(const $::URI::View &resource, const XLSP::Position &position) const {
    return syntax_node_at(resource, position, [](const Syntax::Node *node) { return node->is<Ts...>(); });
  }

  /**
   * @brief Gets definitions comments from a mirror.
   * @param mirror                    Mirror to resolve.
   */
  Relint::Comments comments_view_at(const Relint::Mirror *mirror) const;

  /**
   * @brief Gets a variable definition node at a location.
   * @param resource                  Resource value.
   * @param position                  Position value.
   */
  const Relint::Mirror *vardef_node_at(const Relint::Mirror *mirror) const;
  const Relint::Mirror *vardef_node_at(const $::URI::View &resource, const XLSP::Position &position) const;

  /**
   * @brief Gets a type-definition node at a location.
   * @param resource                  Resource value.
   * @param position                  Position value.
   */
  const Relint::Mirror *typedef_node_at(const Relint::Mirror *mirror) const;
  const Relint::Mirror *typedef_node_at(const $::URI::View &resource, const XLSP::Position &position) const;

  /**
   * @brief Gets a definition node at a location.
   * @param resource                  Resource value.
   * @param position                  Position value.
   */
  const Relint::Mirror *anydef_node_at(const Relint::Mirror *mirror) const;
  const Relint::Mirror *anydef_node_at(const $::URI::View &resource, const XLSP::Position &position) const;
};

} // namespace Sabre::Server

#endif
