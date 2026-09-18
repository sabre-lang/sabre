#ifndef _SABRE_SERVER_STYLE_HPP
#define _SABRE_SERVER_STYLE_HPP

/// Sabre Includes
#include "sabre/forward/server.hpp"
#include "sabre/relint/mirror.hpp"

namespace Sabre::Server::Style {

//  PUBLIC METHODS  //

/**
 * @brief Gets the styled typing for a node.
 * @param mirror                Syntax mirror.
 */
$::String::Buffer typing(const Relint::Mirror *mirror);

/**
 * @brief Handles notating hover content.
 * @param segments              Content to notate.
 */
$::String::Buffer hover(const $::String::View &content);
$::String::Buffer hover(const $::Notate::Segments &segments);
$::String::Buffer hover(const std::vector<$::String::Buffer> &segments);

} // namespace Sabre::Server::Style

#endif
