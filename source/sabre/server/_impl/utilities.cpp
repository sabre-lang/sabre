/// Sabre Includes
#include "sabre/server/utilities.hpp"
#include "sabre/document/service.hpp"
#include "sabre/import/service.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/type/metadata.hpp"

//  CONSTRUCTORS  //

Sabre::Server::Utilities::Utilities() : Utilities($::Global::get<Runtime::Container>()) {}
Sabre::Server::Utilities::Utilities(XI::Container *services) : m_services(services) {}

//  PUBLIC METHODS  //

XLSP::Reference Sabre::Server::Utilities::syntax_to_client(const Relint::Mirror *mirror) const {
  auto *tree = mirror->parent<Syntax::Tree>(); // resolve the tree node
  if ($_UNLIKELY(tree == nullptr)) return syntax_to_client(mirror->origin());
  else return syntax_to_client(mirror->origin(), tree->resource().buffer());
}

XLSP::Reference
Sabre::Server::Utilities::syntax_to_client(const Syntax::Node *node, const $::URI::Buffer &resource) const {
  return {resource, node->trivia()->range().client()};
}

const Sabre::Relint::Exports *Sabre::Server::Utilities::syntax_view_at(const $::URI::View &resource) const {
  Import::Service *modules = *m_services; // resolve
  auto *found = modules->storage()->lookup(resource);
  if (found == nullptr) return nullptr; // failed
  auto *metadata = found->metadata<Module::Phase::TYPED>();
  return metadata->mirrors().get(); // resolve exports
}

const Sabre::Relint::Mirror *Sabre::Server::Utilities::syntax_node_at(
    const $::URI::View &resource, const XLSP::Position &position, Relint::Filter &&filter
) const {
  auto *mirrors = syntax_view_at(resource);
  if (mirrors == nullptr) return nullptr;
  return mirrors->search(position.server(), std::move(filter));
}

Sabre::Relint::Comments Sabre::Server::Utilities::comments_view_at(const Relint::Mirror *mirror) const {
  if (mirror == nullptr) return Relint::Comments();
  auto *definition = anydef_node_at(mirror);
  return definition ? definition->comments() : Relint::Comments();
}

const Sabre::Relint::Mirror *Sabre::Server::Utilities::vardef_node_at(const Relint::Mirror *mirror) const {
  return mirror && !mirror->qualified() ? mirror->definition()->variable : nullptr;
}

const Sabre::Relint::Mirror *
Sabre::Server::Utilities::vardef_node_at(const $::URI::View &resource, const XLSP::Position &position) const {
  return vardef_node_at(syntax_node_at<Syntax::Identifier, Syntax::Declaration>(resource, position));
}

const Sabre::Relint::Mirror *Sabre::Server::Utilities::typedef_node_at(const Relint::Mirror *mirror) const {
  return mirror && mirror->qualified() ? mirror->definition()->annotation : nullptr;
}

const Sabre::Relint::Mirror *
Sabre::Server::Utilities::typedef_node_at(const $::URI::View &resource, const XLSP::Position &position) const {
  return typedef_node_at(syntax_node_at<Syntax::Identifier, Syntax::Alias>(resource, position));
}

const Sabre::Relint::Mirror *Sabre::Server::Utilities::anydef_node_at(const Relint::Mirror *mirror) const {
  if (mirror == nullptr) return nullptr; // stop early when the incoming mirror is invalid at all
  return mirror->qualified() ? mirror->definition()->annotation : mirror->definition()->variable;
}

const Sabre::Relint::Mirror *
Sabre::Server::Utilities::anydef_node_at(const $::URI::View &resource, const XLSP::Position &position) const {
  return anydef_node_at(syntax_node_at<Syntax::Identifier, Syntax::Declaration>(resource, position));
}
